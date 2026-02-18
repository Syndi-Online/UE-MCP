// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IActorModule;

class FBatchSetActorTransformsImplTool : public IMCPTool
{
public:
	explicit FBatchSetActorTransformsImplTool(IActorModule& InActorModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IActorModule& ActorModule;
};
