// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IConsoleModule.h"

class FConsoleImplModule : public IConsoleModule
{
public:
	virtual FConsoleCommandResult ExecuteConsoleCommand(const FString& Command) override;
	virtual FCVarResult GetCVar(const FString& Name) override;
	virtual FCVarResult SetCVar(const FString& Name, const FString& Value) override;
};
