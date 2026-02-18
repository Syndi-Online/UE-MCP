// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FSCCOperationResult
{
	bool bSuccess = false;
	TArray<FString> AffectedFiles;
	FString Message;
	FString ErrorMessage;
};

struct FSCCStatusResult
{
	bool bSuccess = false;

	struct FFileStatus
	{
		FString FilePath;
		bool bIsSourceControlled = false;
		bool bIsCheckedOut = false;
		bool bIsCheckedOutByOther = false;
		FString OtherUser;
		bool bIsAdded = false;
		bool bIsDeleted = false;
		bool bIsModified = false;
		bool bIsCurrent = false;
		bool bCanCheckIn = false;
		bool bCanCheckout = false;
	};

	TArray<FFileStatus> FileStatuses;
	FString ErrorMessage;
};

/**
 * Module interface for Source Control operations.
 */
class ISCCModule
{
public:
	virtual ~ISCCModule() = default;

	virtual FSCCOperationResult Checkout(const TArray<FString>& Files) = 0;
	virtual FSCCOperationResult Add(const TArray<FString>& Files) = 0;
	virtual FSCCOperationResult Delete(const TArray<FString>& Files) = 0;
	virtual FSCCOperationResult Revert(const TArray<FString>& Files) = 0;
	virtual FSCCStatusResult Status(const TArray<FString>& Files) = 0;
	virtual FSCCOperationResult Submit(const TArray<FString>& Files, const FString& Description) = 0;
	virtual FSCCOperationResult Sync(const TArray<FString>& Files) = 0;
};
