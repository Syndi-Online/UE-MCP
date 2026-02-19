// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IBlueprintModule;

class FSetNodePositionImplTool : public IMCPTool
{
public:
	explicit FSetNodePositionImplTool(IBlueprintModule& InBlueprintModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IBlueprintModule& BlueprintModule;
};
