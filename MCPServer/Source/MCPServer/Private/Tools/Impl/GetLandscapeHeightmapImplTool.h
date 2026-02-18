// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ILandscapeToolModule;

class FGetLandscapeHeightmapImplTool : public IMCPTool
{
public:
	explicit FGetLandscapeHeightmapImplTool(ILandscapeToolModule& InLandscapeModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ILandscapeToolModule& LandscapeModule;
};
