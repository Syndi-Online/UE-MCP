// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IFoliageModule;

class FResimulateProceduralFoliageImplTool : public IMCPTool
{
public:
	explicit FResimulateProceduralFoliageImplTool(IFoliageModule& InFoliageModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IFoliageModule& FoliageModule;
};
