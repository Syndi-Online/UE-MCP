// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ISkeletalMeshModule.h"

class FSkeletalMeshImplModule : public ISkeletalMeshModule
{
public:
	virtual FRegenerateSkeletalLodResult RegenerateSkeletalLod(const FString& MeshPath, int32 NewLodCount, bool bRegenerateEvenIfImported) override;
	virtual FSkeletonInfoResult GetSkeletonInfo(const FString& MeshPath) override;
	virtual FGetPhysicsAssetResult GetPhysicsAsset(const FString& MeshPath) override;
	virtual FSetPhysicsAssetResult SetPhysicsAsset(const FString& MeshPath, const FString& PhysicsAssetPath) override;
	virtual FReimportSkeletalMeshResult ReimportSkeletalMesh(const FString& MeshPath) override;
	virtual FCreateAnimAssetResult CreateAnimAsset(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath, const FString& AssetType) override;
	virtual FCreateAnimBlueprintResult CreateAnimBlueprint(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath) override;
	virtual FApplyAnimCompressionResult ApplyAnimCompression(const FString& AnimSequencePath, const FString& CompressionSettingsPath) override;
};
