// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IFoliageModule.h"
#include "MockCallRecorder.h"

class FMockFoliageModule : public IFoliageModule
{
public:
	FMockCallRecorder Recorder;

	FFoliageAddInstancesResult AddFoliageInstancesResult;
	FFoliageRemoveInstancesResult RemoveFoliageInstancesResult;
	FFoliageRegisterMeshResult RegisterFoliageMeshResult;
	FFoliageResimulateResult ResimulateProceduralFoliageResult;

	virtual FFoliageAddInstancesResult AddFoliageInstances(const FString& MeshPath, const TArray<FTransform>& Transforms) override { Recorder.RecordCall(TEXT("AddFoliageInstances")); return AddFoliageInstancesResult; }
	virtual FFoliageRemoveInstancesResult RemoveFoliageInstances(const FString& MeshPath, const FVector& Center, float Radius) override { Recorder.RecordCall(TEXT("RemoveFoliageInstances")); return RemoveFoliageInstancesResult; }
	virtual FFoliageRegisterMeshResult RegisterFoliageMesh(const FString& MeshPath) override { Recorder.RecordCall(TEXT("RegisterFoliageMesh")); return RegisterFoliageMeshResult; }
	virtual FFoliageResimulateResult ResimulateProceduralFoliage(const FString& VolumeName) override { Recorder.RecordCall(TEXT("ResimulateProceduralFoliage")); return ResimulateProceduralFoliageResult; }
};
