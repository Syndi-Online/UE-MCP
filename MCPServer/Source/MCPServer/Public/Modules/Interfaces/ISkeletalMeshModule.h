// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FRegenerateSkeletalLodResult
{
	bool bSuccess = false;
	int32 NewLodCount = 0;
	FString ErrorMessage;
};

struct FBoneInfo
{
	int32 BoneIndex = 0;
	FString BoneName;
	int32 ParentIndex = -1;
};

struct FSkeletonInfoResult
{
	bool bSuccess = false;
	int32 NumBones = 0;
	int32 NumRawBones = 0;
	FString SkeletonName;
	TArray<FBoneInfo> Bones;
	int32 LodCount = 0;
	TArray<int32> VertexCountPerLod;
	FString ErrorMessage;
};

struct FGetPhysicsAssetResult
{
	bool bSuccess = false;
	FString PhysicsAssetPath;
	FString PhysicsAssetName;
	int32 NumBodies = 0;
	int32 NumConstraints = 0;
	FString ErrorMessage;
};

struct FSetPhysicsAssetResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FReimportSkeletalMeshResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FCreateAnimAssetResult
{
	bool bSuccess = false;
	FString AssetPath;
	FString AssetName;
	FString ErrorMessage;
};

struct FCreateAnimBlueprintResult
{
	bool bSuccess = false;
	FString AssetPath;
	FString AssetName;
	FString ErrorMessage;
};

struct FApplyAnimCompressionResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

/**
 * Module interface for Skeletal Mesh and Animation operations.
 */
class ISkeletalMeshModule
{
public:
	virtual ~ISkeletalMeshModule() = default;

	virtual FRegenerateSkeletalLodResult RegenerateSkeletalLod(const FString& MeshPath, int32 NewLodCount, bool bRegenerateEvenIfImported) = 0;
	virtual FSkeletonInfoResult GetSkeletonInfo(const FString& MeshPath) = 0;
	virtual FGetPhysicsAssetResult GetPhysicsAsset(const FString& MeshPath) = 0;
	virtual FSetPhysicsAssetResult SetPhysicsAsset(const FString& MeshPath, const FString& PhysicsAssetPath) = 0;
	virtual FReimportSkeletalMeshResult ReimportSkeletalMesh(const FString& MeshPath) = 0;
	virtual FCreateAnimAssetResult CreateAnimAsset(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath, const FString& AssetType) = 0;
	virtual FCreateAnimBlueprintResult CreateAnimBlueprint(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath) = 0;
	virtual FApplyAnimCompressionResult ApplyAnimCompression(const FString& AnimSequencePath, const FString& CompressionSettingsPath) = 0;
};
