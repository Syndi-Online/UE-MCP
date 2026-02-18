// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FConsoleCommandResult
{
	bool bSuccess = false;
	FString Output;
	FString ErrorMessage;
};

struct FCVarResult
{
	bool bSuccess = false;
	FString Name;
	FString Value;
	FString ErrorMessage;
};

/**
 * Module interface for Console Command operations.
 */
class IConsoleModule
{
public:
	virtual ~IConsoleModule() = default;

	virtual FConsoleCommandResult ExecuteConsoleCommand(const FString& Command) = 0;
	virtual FCVarResult GetCVar(const FString& Name) = 0;
	virtual FCVarResult SetCVar(const FString& Name, const FString& Value) = 0;
};
