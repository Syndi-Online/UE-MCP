// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ISkeletalMeshModule;

class FReimportSkeletalMeshImplTool : public IMCPTool
{
public:
	explicit FReimportSkeletalMeshImplTool(ISkeletalMeshModule& InSkeletalMeshModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ISkeletalMeshModule& SkeletalMeshModule;
};
