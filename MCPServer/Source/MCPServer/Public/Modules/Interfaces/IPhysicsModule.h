// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FPhysicsCollisionResult
{
	bool bSuccess = false;
	int32 ShapesAdded = 0;
	FString ErrorMessage;
};

struct FPhysicalMaterialResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FPhysicsVelocityResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FPhysicsConstraintResult
{
	bool bSuccess = false;
	FString ConstraintName;
	FString ErrorMessage;
};

/**
 * Module interface for Physics operations.
 */
class IPhysicsModule
{
public:
	virtual ~IPhysicsModule() = default;

	virtual FPhysicsCollisionResult CreatePhysicsCollision(const FString& ActorName, const FString& ShapeType, const FVector& Dimensions, const FVector& Offset) = 0;
	virtual FPhysicalMaterialResult SetPhysicalMaterial(const FString& ActorName, const FString& MaterialPath) = 0;
	virtual FPhysicsVelocityResult SetPhysicsVelocity(const FString& ActorName, const FVector& LinearVelocity, const FVector& AngularVelocity, bool bEnablePhysics) = 0;
	virtual FPhysicsConstraintResult ManageConstraints(const FString& ActorName1, const FString& ActorName2, const FString& ConstraintType, bool bDisableCollision) = 0;
};
