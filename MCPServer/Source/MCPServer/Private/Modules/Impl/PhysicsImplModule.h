// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IPhysicsModule.h"

class IActorModule;

class FPhysicsImplModule : public IPhysicsModule
{
public:
	explicit FPhysicsImplModule(IActorModule& InActorModule);

	virtual FPhysicsCollisionResult CreatePhysicsCollision(const FString& ActorName, const FString& ShapeType, const FVector& Dimensions, const FVector& Offset) override;
	virtual FPhysicalMaterialResult SetPhysicalMaterial(const FString& ActorName, const FString& MaterialPath) override;
	virtual FPhysicsVelocityResult SetPhysicsVelocity(const FString& ActorName, const FVector& LinearVelocity, const FVector& AngularVelocity, bool bEnablePhysics) override;
	virtual FPhysicsConstraintResult ManageConstraints(const FString& ActorName1, const FString& ActorName2, const FString& ConstraintType, bool bDisableCollision) override;

private:
	IActorModule& ActorModule;
};
