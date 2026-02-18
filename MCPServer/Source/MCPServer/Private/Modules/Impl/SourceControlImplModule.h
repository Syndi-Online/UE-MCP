// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ISourceControlModule.h"

class FSourceControlImplModule : public ISCCModule
{
public:
	virtual FSCCOperationResult Checkout(const TArray<FString>& Files) override;
	virtual FSCCOperationResult Add(const TArray<FString>& Files) override;
	virtual FSCCOperationResult Delete(const TArray<FString>& Files) override;
	virtual FSCCOperationResult Revert(const TArray<FString>& Files) override;
	virtual FSCCStatusResult Status(const TArray<FString>& Files) override;
	virtual FSCCOperationResult Submit(const TArray<FString>& Files, const FString& Description) override;
	virtual FSCCOperationResult Sync(const TArray<FString>& Files) override;
};
