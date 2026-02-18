// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IPhysicsModule.h"
#include "MockCallRecorder.h"

class FMockPhysicsModule : public IPhysicsModule
{
public:
	FMockCallRecorder Recorder;

	FPhysicsCollisionResult CreatePhysicsCollisionResult;
	FPhysicalMaterialResult SetPhysicalMaterialResult;
	FPhysicsVelocityResult SetPhysicsVelocityResult;
	FPhysicsConstraintResult ManageConstraintsResult;

	virtual FPhysicsCollisionResult CreatePhysicsCollision(const FString& ActorName, const FString& ShapeType, const FVector& Dimensions, const FVector& Offset) override { Recorder.RecordCall(TEXT("CreatePhysicsCollision")); return CreatePhysicsCollisionResult; }
	virtual FPhysicalMaterialResult SetPhysicalMaterial(const FString& ActorName, const FString& MaterialPath) override { Recorder.RecordCall(TEXT("SetPhysicalMaterial")); return SetPhysicalMaterialResult; }
	virtual FPhysicsVelocityResult SetPhysicsVelocity(const FString& ActorName, const FVector& LinearVelocity, const FVector& AngularVelocity, bool bEnablePhysics) override { Recorder.RecordCall(TEXT("SetPhysicsVelocity")); return SetPhysicsVelocityResult; }
	virtual FPhysicsConstraintResult ManageConstraints(const FString& ActorName1, const FString& ActorName2, const FString& ConstraintType, bool bDisableCollision) override { Recorder.RecordCall(TEXT("ManageConstraints")); return ManageConstraintsResult; }
};
