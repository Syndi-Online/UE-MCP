// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IBuildModule;

class FBuildTextureStreamingImplTool : public IMCPTool
{
public:
	explicit FBuildTextureStreamingImplTool(IBuildModule& InBuildModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IBuildModule& BuildModule;
};
