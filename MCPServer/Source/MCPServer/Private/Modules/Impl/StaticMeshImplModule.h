// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IStaticMeshModule.h"

class FStaticMeshImplModule : public IStaticMeshModule
{
public:
	virtual FSetStaticMeshLodResult SetStaticMeshLod(const FString& MeshPath, const TArray<float>& ScreenSizes) override;
	virtual FImportLodResult ImportLod(const FString& MeshPath, int32 LodIndex, const FString& SourceFilePath) override;
	virtual FGetLodSettingsResult GetLodSettings(const FString& MeshPath) override;
	virtual FSetCollisionResult SetCollision(const FString& MeshPath, const FString& CollisionType) override;
	virtual FUVChannelResult AddUVChannel(const FString& MeshPath, int32 LodIndex) override;
	virtual FUVChannelResult RemoveUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex) override;
	virtual FUVChannelResult GenerateUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex, const FString& ProjectionType) override;
	virtual FSetMeshMaterialResult SetMeshMaterial(const FString& MeshPath, int32 MaterialIndex, const FString& MaterialPath) override;
	virtual FMeshBoundsResult GetMeshBounds(const FString& MeshPath) override;
};
