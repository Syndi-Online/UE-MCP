// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ISkeletalMeshModule;

class FCreateAnimAssetImplTool : public IMCPTool
{
public:
	explicit FCreateAnimAssetImplTool(ISkeletalMeshModule& InSkeletalMeshModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ISkeletalMeshModule& SkeletalMeshModule;
};
