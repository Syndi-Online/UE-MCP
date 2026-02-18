// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FSetStaticMeshLodResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FImportLodResult
{
	bool bSuccess = false;
	int32 LodIndex = -1;
	FString ErrorMessage;
};

struct FLodInfo
{
	int32 LodIndex = 0;
	float ScreenSize = 0.0f;
	int32 NumVertices = 0;
	int32 NumTriangles = 0;
};

struct FGetLodSettingsResult
{
	bool bSuccess = false;
	int32 LodCount = 0;
	FString LodGroup;
	TArray<FLodInfo> Lods;
	FString ErrorMessage;
};

struct FSetCollisionResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FUVChannelResult
{
	bool bSuccess = false;
	int32 NumUVChannels = 0;
	FString ErrorMessage;
};

struct FSetMeshMaterialResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FMeshBoundsResult
{
	bool bSuccess = false;
	FVector Origin = FVector::ZeroVector;
	FVector BoxExtent = FVector::ZeroVector;
	float SphereRadius = 0.0f;
	FString ErrorMessage;
};

/**
 * Module interface for Static Mesh operations.
 */
class IStaticMeshModule
{
public:
	virtual ~IStaticMeshModule() = default;

	virtual FSetStaticMeshLodResult SetStaticMeshLod(const FString& MeshPath, const TArray<float>& ScreenSizes) = 0;
	virtual FImportLodResult ImportLod(const FString& MeshPath, int32 LodIndex, const FString& SourceFilePath) = 0;
	virtual FGetLodSettingsResult GetLodSettings(const FString& MeshPath) = 0;
	virtual FSetCollisionResult SetCollision(const FString& MeshPath, const FString& CollisionType) = 0;
	virtual FUVChannelResult AddUVChannel(const FString& MeshPath, int32 LodIndex) = 0;
	virtual FUVChannelResult RemoveUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex) = 0;
	virtual FUVChannelResult GenerateUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex, const FString& ProjectionType) = 0;
	virtual FSetMeshMaterialResult SetMeshMaterial(const FString& MeshPath, int32 MaterialIndex, const FString& MaterialPath) = 0;
	virtual FMeshBoundsResult GetMeshBounds(const FString& MeshPath) = 0;
};
