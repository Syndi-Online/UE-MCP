// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FFoliageAddInstancesResult
{
	bool bSuccess = false;
	int32 InstancesAdded = 0;
	FString ErrorMessage;
};

struct FFoliageRemoveInstancesResult
{
	bool bSuccess = false;
	int32 InstancesRemoved = 0;
	FString ErrorMessage;
};

struct FFoliageRegisterMeshResult
{
	bool bSuccess = false;
	FString FoliageTypeName;
	FString ErrorMessage;
};

struct FFoliageResimulateResult
{
	bool bSuccess = false;
	int32 VolumesProcessed = 0;
	FString ErrorMessage;
};

/**
 * Module interface for Foliage operations.
 */
class IFoliageModule
{
public:
	virtual ~IFoliageModule() = default;

	virtual FFoliageAddInstancesResult AddFoliageInstances(const FString& MeshPath, const TArray<FTransform>& Transforms) = 0;
	virtual FFoliageRemoveInstancesResult RemoveFoliageInstances(const FString& MeshPath, const FVector& Center, float Radius) = 0;
	virtual FFoliageRegisterMeshResult RegisterFoliageMesh(const FString& MeshPath) = 0;
	virtual FFoliageResimulateResult ResimulateProceduralFoliage(const FString& VolumeName) = 0;
};
