// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IPhysicsModule;

class FSetPhysicalMaterialImplTool : public IMCPTool
{
public:
	explicit FSetPhysicalMaterialImplTool(IPhysicsModule& InPhysicsModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IPhysicsModule& PhysicsModule;
};
