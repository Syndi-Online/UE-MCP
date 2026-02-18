// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IFoliageModule.h"

class FFoliageImplModule : public IFoliageModule
{
public:
	virtual FFoliageAddInstancesResult AddFoliageInstances(const FString& MeshPath, const TArray<FTransform>& Transforms) override;
	virtual FFoliageRemoveInstancesResult RemoveFoliageInstances(const FString& MeshPath, const FVector& Center, float Radius) override;
	virtual FFoliageRegisterMeshResult RegisterFoliageMesh(const FString& MeshPath) override;
	virtual FFoliageResimulateResult ResimulateProceduralFoliage(const FString& VolumeName) override;
};
