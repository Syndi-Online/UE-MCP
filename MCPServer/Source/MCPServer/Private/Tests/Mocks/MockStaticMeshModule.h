// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IStaticMeshModule.h"
#include "MockCallRecorder.h"

class FMockStaticMeshModule : public IStaticMeshModule
{
public:
	FMockCallRecorder Recorder;

	FSetStaticMeshLodResult SetStaticMeshLodResult;
	FImportLodResult ImportLodResult;
	FGetLodSettingsResult GetLodSettingsResult;
	FSetCollisionResult SetCollisionResult;
	FUVChannelResult AddUVChannelResult;
	FUVChannelResult RemoveUVChannelResult;
	FUVChannelResult GenerateUVChannelResult;
	FSetMeshMaterialResult SetMeshMaterialResult;
	FMeshBoundsResult GetMeshBoundsResult;

	virtual FSetStaticMeshLodResult SetStaticMeshLod(const FString& MeshPath, const TArray<float>& ScreenSizes) override { Recorder.RecordCall(TEXT("SetStaticMeshLod")); return SetStaticMeshLodResult; }
	virtual FImportLodResult ImportLod(const FString& MeshPath, int32 LodIndex, const FString& SourceFilePath) override { Recorder.RecordCall(TEXT("ImportLod")); return ImportLodResult; }
	virtual FGetLodSettingsResult GetLodSettings(const FString& MeshPath) override { Recorder.RecordCall(TEXT("GetLodSettings")); return GetLodSettingsResult; }
	virtual FSetCollisionResult SetCollision(const FString& MeshPath, const FString& CollisionType) override { Recorder.RecordCall(TEXT("SetCollision")); return SetCollisionResult; }
	virtual FUVChannelResult AddUVChannel(const FString& MeshPath, int32 LodIndex) override { Recorder.RecordCall(TEXT("AddUVChannel")); return AddUVChannelResult; }
	virtual FUVChannelResult RemoveUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex) override { Recorder.RecordCall(TEXT("RemoveUVChannel")); return RemoveUVChannelResult; }
	virtual FUVChannelResult GenerateUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex, const FString& ProjectionType) override { Recorder.RecordCall(TEXT("GenerateUVChannel")); return GenerateUVChannelResult; }
	virtual FSetMeshMaterialResult SetMeshMaterial(const FString& MeshPath, int32 MaterialIndex, const FString& MaterialPath) override { Recorder.RecordCall(TEXT("SetMeshMaterial")); return SetMeshMaterialResult; }
	virtual FMeshBoundsResult GetMeshBounds(const FString& MeshPath) override { Recorder.RecordCall(TEXT("GetMeshBounds")); return GetMeshBoundsResult; }
};
