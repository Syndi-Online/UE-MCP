// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IFoliageModule;

class FRemoveFoliageInstancesImplTool : public IMCPTool
{
public:
	explicit FRemoveFoliageInstancesImplTool(IFoliageModule& InFoliageModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IFoliageModule& FoliageModule;
};
