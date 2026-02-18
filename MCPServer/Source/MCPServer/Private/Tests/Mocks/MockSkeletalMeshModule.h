// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "MockCallRecorder.h"

class FMockSkeletalMeshModule : public ISkeletalMeshModule
{
public:
	FMockCallRecorder Recorder;

	FRegenerateSkeletalLodResult RegenerateSkeletalLodResult;
	FSkeletonInfoResult GetSkeletonInfoResult;
	FGetPhysicsAssetResult GetPhysicsAssetResult;
	FSetPhysicsAssetResult SetPhysicsAssetResult;
	FReimportSkeletalMeshResult ReimportSkeletalMeshResult;
	FCreateAnimAssetResult CreateAnimAssetResult;
	FCreateAnimBlueprintResult CreateAnimBlueprintResult;
	FApplyAnimCompressionResult ApplyAnimCompressionResult;

	virtual FRegenerateSkeletalLodResult RegenerateSkeletalLod(const FString& MeshPath, int32 NewLodCount, bool bRegenerateEvenIfImported) override { Recorder.RecordCall(TEXT("RegenerateSkeletalLod")); return RegenerateSkeletalLodResult; }
	virtual FSkeletonInfoResult GetSkeletonInfo(const FString& MeshPath) override { Recorder.RecordCall(TEXT("GetSkeletonInfo")); return GetSkeletonInfoResult; }
	virtual FGetPhysicsAssetResult GetPhysicsAsset(const FString& MeshPath) override { Recorder.RecordCall(TEXT("GetPhysicsAsset")); return GetPhysicsAssetResult; }
	virtual FSetPhysicsAssetResult SetPhysicsAsset(const FString& MeshPath, const FString& PhysicsAssetPath) override { Recorder.RecordCall(TEXT("SetPhysicsAsset")); return SetPhysicsAssetResult; }
	virtual FReimportSkeletalMeshResult ReimportSkeletalMesh(const FString& MeshPath) override { Recorder.RecordCall(TEXT("ReimportSkeletalMesh")); return ReimportSkeletalMeshResult; }
	virtual FCreateAnimAssetResult CreateAnimAsset(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath, const FString& AssetType) override { Recorder.RecordCall(TEXT("CreateAnimAsset")); return CreateAnimAssetResult; }
	virtual FCreateAnimBlueprintResult CreateAnimBlueprint(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath) override { Recorder.RecordCall(TEXT("CreateAnimBlueprint")); return CreateAnimBlueprintResult; }
	virtual FApplyAnimCompressionResult ApplyAnimCompression(const FString& AnimSequencePath, const FString& CompressionSettingsPath) override { Recorder.RecordCall(TEXT("ApplyAnimCompression")); return ApplyAnimCompressionResult; }
};
