// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IActorModule;

class FSetActorPropertyImplTool : public IMCPTool
{
public:
	explicit FSetActorPropertyImplTool(IActorModule& InActorModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IActorModule& ActorModule;
};
