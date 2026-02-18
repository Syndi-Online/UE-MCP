// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IWorldPartitionModule;

class FDeleteHlodWpImplTool : public IMCPTool
{
public:
	explicit FDeleteHlodWpImplTool(IWorldPartitionModule& InWorldPartitionModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IWorldPartitionModule& WorldPartitionModule;
};
