// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IPhysicsModule;

class FCreatePhysicsCollisionImplTool : public IMCPTool
{
public:
	explicit FCreatePhysicsCollisionImplTool(IPhysicsModule& InPhysicsModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IPhysicsModule& PhysicsModule;
};
