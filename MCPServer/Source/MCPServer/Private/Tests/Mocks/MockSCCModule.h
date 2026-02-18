// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ISourceControlModule.h"
#include "MockCallRecorder.h"

class FMockSCCModule : public ISCCModule
{
public:
	FMockCallRecorder Recorder;

	FSCCOperationResult CheckoutResult;
	FSCCOperationResult AddResult;
	FSCCOperationResult DeleteResult;
	FSCCOperationResult RevertResult;
	FSCCStatusResult StatusResult;
	FSCCOperationResult SubmitResult;
	FSCCOperationResult SyncResult;

	virtual FSCCOperationResult Checkout(const TArray<FString>& Files) override { Recorder.RecordCall(TEXT("Checkout")); return CheckoutResult; }
	virtual FSCCOperationResult Add(const TArray<FString>& Files) override { Recorder.RecordCall(TEXT("Add")); return AddResult; }
	virtual FSCCOperationResult Delete(const TArray<FString>& Files) override { Recorder.RecordCall(TEXT("Delete")); return DeleteResult; }
	virtual FSCCOperationResult Revert(const TArray<FString>& Files) override { Recorder.RecordCall(TEXT("Revert")); return RevertResult; }
	virtual FSCCStatusResult Status(const TArray<FString>& Files) override { Recorder.RecordCall(TEXT("Status")); return StatusResult; }
	virtual FSCCOperationResult Submit(const TArray<FString>& Files, const FString& Description) override { Recorder.RecordCall(TEXT("Submit")); return SubmitResult; }
	virtual FSCCOperationResult Sync(const TArray<FString>& Files) override { Recorder.RecordCall(TEXT("Sync")); return SyncResult; }
};
