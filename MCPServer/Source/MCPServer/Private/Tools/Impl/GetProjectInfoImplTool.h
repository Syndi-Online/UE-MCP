// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Tools/Interfaces/IMCPTool.h"

class IProjectSettingsModule;

class FGetProjectInfoImplTool : public IMCPTool
{
public:
	explicit FGetProjectInfoImplTool(IProjectSettingsModule& InModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IProjectSettingsModule& ProjectSettingsModule;
};
