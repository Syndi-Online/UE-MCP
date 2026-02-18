// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IAssetModule;

class FSetAssetMetadataImplTool : public IMCPTool
{
public:
	explicit FSetAssetMetadataImplTool(IAssetModule& InAssetModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IAssetModule& AssetModule;
};
