// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Tools/Interfaces/IMCPTool.h"

class IConsoleModule;

class FSetCvarImplTool : public IMCPTool
{
public:
	explicit FSetCvarImplTool(IConsoleModule& InConsoleModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IConsoleModule& ConsoleModule;
};
