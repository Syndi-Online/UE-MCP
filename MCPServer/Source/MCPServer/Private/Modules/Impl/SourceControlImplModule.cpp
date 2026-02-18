// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/SourceControlImplModule.h"
#include "ISourceControlModule.h"
#include "ISourceControlProvider.h"
#include "SourceControlOperations.h"
#include "ISourceControlState.h"

static bool GetProviderChecked(ISourceControlProvider*& OutProvider, FString& OutError)
{
	ISourceControlModule* SCCModule = ISourceControlModule::GetPtr();
	if (!SCCModule || !SCCModule->IsEnabled())
	{
		OutError = TEXT("Source control is not enabled");
		return false;
	}

	OutProvider = &SCCModule->GetProvider();
	if (!OutProvider->IsAvailable())
	{
		OutError = TEXT("Source control provider is not available");
		return false;
	}
	return true;
}

static FString GetErrorString(const FSourceControlOperationRef& Op)
{
	const FSourceControlResultInfo& Info = Op->GetResultInfo();
	FString Combined;
	for (const FText& Err : Info.ErrorMessages)
	{
		if (!Combined.IsEmpty()) Combined += TEXT("; ");
		Combined += Err.ToString();
	}
	return Combined.IsEmpty() ? TEXT("Unknown error") : Combined;
}

FSCCOperationResult FSourceControlImplModule::Checkout(const TArray<FString>& Files)
{
	FSCCOperationResult Result;

	ISourceControlProvider* Provider = nullptr;
	if (!GetProviderChecked(Provider, Result.ErrorMessage))
	{
		return Result;
	}

	FSourceControlOperationRef Op = ISourceControlOperation::Create<FCheckOut>();
	ECommandResult::Type CmdResult = Provider->Execute(Op, Files, EConcurrency::Synchronous);

	Result.bSuccess = (CmdResult == ECommandResult::Succeeded);
	Result.AffectedFiles = Files;
	if (Result.bSuccess)
	{
		Result.Message = FString::Printf(TEXT("Checked out %d file(s)"), Files.Num());
	}
	else
	{
		Result.ErrorMessage = GetErrorString(Op);
	}
	return Result;
}

FSCCOperationResult FSourceControlImplModule::Add(const TArray<FString>& Files)
{
	FSCCOperationResult Result;

	ISourceControlProvider* Provider = nullptr;
	if (!GetProviderChecked(Provider, Result.ErrorMessage))
	{
		return Result;
	}

	FSourceControlOperationRef Op = ISourceControlOperation::Create<FMarkForAdd>();
	ECommandResult::Type CmdResult = Provider->Execute(Op, Files, EConcurrency::Synchronous);

	Result.bSuccess = (CmdResult == ECommandResult::Succeeded);
	Result.AffectedFiles = Files;
	if (Result.bSuccess)
	{
		Result.Message = FString::Printf(TEXT("Marked %d file(s) for add"), Files.Num());
	}
	else
	{
		Result.ErrorMessage = GetErrorString(Op);
	}
	return Result;
}

FSCCOperationResult FSourceControlImplModule::Delete(const TArray<FString>& Files)
{
	FSCCOperationResult Result;

	ISourceControlProvider* Provider = nullptr;
	if (!GetProviderChecked(Provider, Result.ErrorMessage))
	{
		return Result;
	}

	FSourceControlOperationRef Op = ISourceControlOperation::Create<FDelete>();
	ECommandResult::Type CmdResult = Provider->Execute(Op, Files, EConcurrency::Synchronous);

	Result.bSuccess = (CmdResult == ECommandResult::Succeeded);
	Result.AffectedFiles = Files;
	if (Result.bSuccess)
	{
		Result.Message = FString::Printf(TEXT("Marked %d file(s) for delete"), Files.Num());
	}
	else
	{
		Result.ErrorMessage = GetErrorString(Op);
	}
	return Result;
}

FSCCOperationResult FSourceControlImplModule::Revert(const TArray<FString>& Files)
{
	FSCCOperationResult Result;

	ISourceControlProvider* Provider = nullptr;
	if (!GetProviderChecked(Provider, Result.ErrorMessage))
	{
		return Result;
	}

	FSourceControlOperationRef Op = ISourceControlOperation::Create<FRevert>();
	ECommandResult::Type CmdResult = Provider->Execute(Op, Files, EConcurrency::Synchronous);

	Result.bSuccess = (CmdResult == ECommandResult::Succeeded);
	Result.AffectedFiles = Files;
	if (Result.bSuccess)
	{
		Result.Message = FString::Printf(TEXT("Reverted %d file(s)"), Files.Num());
	}
	else
	{
		Result.ErrorMessage = GetErrorString(Op);
	}
	return Result;
}

FSCCStatusResult FSourceControlImplModule::Status(const TArray<FString>& Files)
{
	FSCCStatusResult Result;

	ISourceControlProvider* Provider = nullptr;
	FString Error;
	if (!GetProviderChecked(Provider, Error))
	{
		Result.ErrorMessage = Error;
		return Result;
	}

	// Force update status from server
	FSourceControlOperationRef UpdateOp = ISourceControlOperation::Create<FUpdateStatus>();
	Provider->Execute(UpdateOp, Files, EConcurrency::Synchronous);

	TArray<FSourceControlStateRef> States;
	ECommandResult::Type CmdResult = Provider->GetState(Files, States, EStateCacheUsage::Use);

	if (CmdResult != ECommandResult::Succeeded)
	{
		Result.ErrorMessage = TEXT("Failed to query file states");
		return Result;
	}

	Result.bSuccess = true;
	for (const FSourceControlStateRef& State : States)
	{
		FSCCStatusResult::FFileStatus FileStatus;
		FileStatus.FilePath = State->GetFilename();
		FileStatus.bIsSourceControlled = State->IsSourceControlled();
		FileStatus.bIsCheckedOut = State->IsCheckedOut();
		FileStatus.bIsCheckedOutByOther = State->IsCheckedOutOther(&FileStatus.OtherUser);
		FileStatus.bIsAdded = State->IsAdded();
		FileStatus.bIsDeleted = State->IsDeleted();
		FileStatus.bIsModified = State->IsModified();
		FileStatus.bIsCurrent = State->IsCurrent();
		FileStatus.bCanCheckIn = State->CanCheckIn();
		FileStatus.bCanCheckout = State->CanCheckout();
		Result.FileStatuses.Add(MoveTemp(FileStatus));
	}
	return Result;
}

FSCCOperationResult FSourceControlImplModule::Submit(const TArray<FString>& Files, const FString& Description)
{
	FSCCOperationResult Result;

	ISourceControlProvider* Provider = nullptr;
	if (!GetProviderChecked(Provider, Result.ErrorMessage))
	{
		return Result;
	}

	TSharedRef<FCheckIn, ESPMode::ThreadSafe> CheckInOp = ISourceControlOperation::Create<FCheckIn>();
	CheckInOp->SetDescription(FText::FromString(Description));

	ECommandResult::Type CmdResult = Provider->Execute(CheckInOp, Files, EConcurrency::Synchronous);

	Result.bSuccess = (CmdResult == ECommandResult::Succeeded);
	Result.AffectedFiles = Files;
	if (Result.bSuccess)
	{
		Result.Message = CheckInOp->GetSuccessMessage().ToString();
		if (Result.Message.IsEmpty())
		{
			Result.Message = FString::Printf(TEXT("Submitted %d file(s)"), Files.Num());
		}
	}
	else
	{
		Result.ErrorMessage = GetErrorString(CheckInOp);
	}
	return Result;
}

FSCCOperationResult FSourceControlImplModule::Sync(const TArray<FString>& Files)
{
	FSCCOperationResult Result;

	ISourceControlProvider* Provider = nullptr;
	if (!GetProviderChecked(Provider, Result.ErrorMessage))
	{
		return Result;
	}

	FSourceControlOperationRef Op = ISourceControlOperation::Create<FSync>();
	ECommandResult::Type CmdResult = Provider->Execute(Op, Files, EConcurrency::Synchronous);

	Result.bSuccess = (CmdResult == ECommandResult::Succeeded);
	Result.AffectedFiles = Files;
	if (Result.bSuccess)
	{
		Result.Message = FString::Printf(TEXT("Synced %d file(s)"), Files.Num());
	}
	else
	{
		Result.ErrorMessage = GetErrorString(Op);
	}
	return Result;
}
