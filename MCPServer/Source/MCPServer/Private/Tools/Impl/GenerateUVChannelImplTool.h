// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IStaticMeshModule;

class FGenerateUVChannelImplTool : public IMCPTool
{
public:
	explicit FGenerateUVChannelImplTool(IStaticMeshModule& InStaticMeshModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IStaticMeshModule& StaticMeshModule;
};
