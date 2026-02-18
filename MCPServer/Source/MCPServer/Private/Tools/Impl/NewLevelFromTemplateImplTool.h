// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ILevelModule;

class FNewLevelFromTemplateImplTool : public IMCPTool
{
public:
	explicit FNewLevelFromTemplateImplTool(ILevelModule& InLevelModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ILevelModule& LevelModule;
};
