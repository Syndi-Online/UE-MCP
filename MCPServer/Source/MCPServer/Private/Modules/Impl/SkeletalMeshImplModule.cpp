// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/SkeletalMeshImplModule.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "SkeletalMeshEditorSubsystem.h"
#include "EditorReimportHandler.h"
#include "Factories/AnimSequenceFactory.h"
#include "Factories/BlendSpaceFactoryNew.h"
#include "Factories/BlendSpaceFactory1D.h"
#include "Factories/AnimMontageFactory.h"
#include "Factories/AnimBlueprintFactory.h"
#include "Animation/AnimSequence.h"
#include "Animation/BlendSpace.h"
#include "Animation/BlendSpace1D.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBoneCompressionSettings.h"
#include "AnimationBlueprintLibrary.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Editor.h"

FRegenerateSkeletalLodResult FSkeletalMeshImplModule::RegenerateSkeletalLod(const FString& MeshPath, int32 NewLodCount, bool bRegenerateEvenIfImported)
{
	FRegenerateSkeletalLodResult Result;

	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeletal mesh not found: %s"), *MeshPath);
		return Result;
	}

	USkeletalMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<USkeletalMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("USkeletalMeshEditorSubsystem is not available");
		return Result;
	}

	bool bSuccess = Subsystem->RegenerateLOD(Mesh, NewLodCount, bRegenerateEvenIfImported, false);
	if (!bSuccess)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to regenerate LOD. Mesh reduction may not be available.");
		return Result;
	}

	Result.bSuccess = true;
	Result.NewLodCount = Subsystem->GetLODCount(Mesh);
	return Result;
}

FSkeletonInfoResult FSkeletalMeshImplModule::GetSkeletonInfo(const FString& MeshPath)
{
	FSkeletonInfoResult Result;

	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeletal mesh not found: %s"), *MeshPath);
		return Result;
	}

	USkeletalMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<USkeletalMeshEditorSubsystem>();

	const FReferenceSkeleton& RefSkeleton = Mesh->GetRefSkeleton();
	Result.NumBones = RefSkeleton.GetNum();
	Result.NumRawBones = RefSkeleton.GetRawBoneNum();

	USkeleton* Skeleton = Mesh->GetSkeleton();
	if (Skeleton)
	{
		Result.SkeletonName = Skeleton->GetPathName();
	}

	for (int32 i = 0; i < RefSkeleton.GetRawBoneNum(); ++i)
	{
		FBoneInfo BoneInfo;
		BoneInfo.BoneIndex = i;
		BoneInfo.BoneName = RefSkeleton.GetBoneName(i).ToString();
		BoneInfo.ParentIndex = RefSkeleton.GetParentIndex(i);
		Result.Bones.Add(BoneInfo);
	}

	if (Subsystem)
	{
		Result.LodCount = Subsystem->GetLODCount(Mesh);
		for (int32 i = 0; i < Result.LodCount; ++i)
		{
			Result.VertexCountPerLod.Add(Subsystem->GetNumVerts(Mesh, i));
		}
	}

	Result.bSuccess = true;
	return Result;
}

FGetPhysicsAssetResult FSkeletalMeshImplModule::GetPhysicsAsset(const FString& MeshPath)
{
	FGetPhysicsAssetResult Result;

	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeletal mesh not found: %s"), *MeshPath);
		return Result;
	}

	UPhysicsAsset* PhysAsset = Mesh->GetPhysicsAsset();
	if (!PhysAsset)
	{
		Result.bSuccess = true;
		Result.PhysicsAssetPath = TEXT("");
		Result.PhysicsAssetName = TEXT("None");
		Result.NumBodies = 0;
		Result.NumConstraints = 0;
		return Result;
	}

	Result.bSuccess = true;
	Result.PhysicsAssetPath = PhysAsset->GetPathName();
	Result.PhysicsAssetName = PhysAsset->GetName();
	Result.NumBodies = PhysAsset->SkeletalBodySetups.Num();
	Result.NumConstraints = PhysAsset->ConstraintSetup.Num();
	return Result;
}

FSetPhysicsAssetResult FSkeletalMeshImplModule::SetPhysicsAsset(const FString& MeshPath, const FString& PhysicsAssetPath)
{
	FSetPhysicsAssetResult Result;

	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeletal mesh not found: %s"), *MeshPath);
		return Result;
	}

	UPhysicsAsset* PhysAsset = nullptr;
	if (!PhysicsAssetPath.IsEmpty())
	{
		PhysAsset = LoadObject<UPhysicsAsset>(nullptr, *PhysicsAssetPath);
		if (!PhysAsset)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Physics asset not found: %s"), *PhysicsAssetPath);
			return Result;
		}
	}

	Mesh->SetPhysicsAsset(PhysAsset);
	Mesh->PostEditChange();
	Mesh->MarkPackageDirty();

	Result.bSuccess = true;
	return Result;
}

FReimportSkeletalMeshResult FSkeletalMeshImplModule::ReimportSkeletalMesh(const FString& MeshPath)
{
	FReimportSkeletalMeshResult Result;

	USkeletalMesh* Mesh = LoadObject<USkeletalMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeletal mesh not found: %s"), *MeshPath);
		return Result;
	}

	FReimportManager* ReimportManager = FReimportManager::Instance();
	if (!ReimportManager)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("FReimportManager is not available");
		return Result;
	}

	bool bSuccess = ReimportManager->Reimport(Mesh, false, true, TEXT(""), nullptr, INDEX_NONE, false, true);
	if (!bSuccess)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Reimport failed. The source file may be missing or invalid.");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FCreateAnimAssetResult FSkeletalMeshImplModule::CreateAnimAsset(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath, const FString& AssetType)
{
	FCreateAnimAssetResult Result;

	USkeleton* Skeleton = LoadObject<USkeleton>(nullptr, *SkeletonPath);
	if (!Skeleton)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeleton not found: %s"), *SkeletonPath);
		return Result;
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();

	UObject* NewAsset = nullptr;

	if (AssetType.Equals(TEXT("AnimSequence"), ESearchCase::IgnoreCase))
	{
		UAnimSequenceFactory* Factory = NewObject<UAnimSequenceFactory>();
		Factory->TargetSkeleton = Skeleton;
		NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UAnimSequence::StaticClass(), Factory);
	}
	else if (AssetType.Equals(TEXT("BlendSpace"), ESearchCase::IgnoreCase))
	{
		UBlendSpaceFactoryNew* Factory = NewObject<UBlendSpaceFactoryNew>();
		Factory->TargetSkeleton = Skeleton;
		NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UBlendSpace::StaticClass(), Factory);
	}
	else if (AssetType.Equals(TEXT("BlendSpace1D"), ESearchCase::IgnoreCase))
	{
		UBlendSpaceFactory1D* Factory = NewObject<UBlendSpaceFactory1D>();
		Factory->TargetSkeleton = Skeleton;
		NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UBlendSpace1D::StaticClass(), Factory);
	}
	else if (AssetType.Equals(TEXT("AnimMontage"), ESearchCase::IgnoreCase))
	{
		UAnimMontageFactory* Factory = NewObject<UAnimMontageFactory>();
		Factory->TargetSkeleton = Skeleton;
		NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, UAnimMontage::StaticClass(), Factory);
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown asset type: %s. Supported: AnimSequence, BlendSpace, BlendSpace1D, AnimMontage"), *AssetType);
		return Result;
	}

	if (!NewAsset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create %s asset"), *AssetType);
		return Result;
	}

	Result.bSuccess = true;
	Result.AssetPath = NewAsset->GetPathName();
	Result.AssetName = NewAsset->GetName();
	return Result;
}

FCreateAnimBlueprintResult FSkeletalMeshImplModule::CreateAnimBlueprint(const FString& SkeletonPath, const FString& AssetName, const FString& PackagePath)
{
	FCreateAnimBlueprintResult Result;

	USkeleton* Skeleton = LoadObject<USkeleton>(nullptr, *SkeletonPath);
	if (!Skeleton)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Skeleton not found: %s"), *SkeletonPath);
		return Result;
	}

	UAnimBlueprintFactory* Factory = NewObject<UAnimBlueprintFactory>();
	Factory->ParentClass = UAnimInstance::StaticClass();
	Factory->TargetSkeleton = Skeleton;

	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	UObject* NewAsset = AssetToolsModule.Get().CreateAsset(AssetName, PackagePath, UAnimBlueprint::StaticClass(), Factory);

	if (!NewAsset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to create Anim Blueprint");
		return Result;
	}

	Result.bSuccess = true;
	Result.AssetPath = NewAsset->GetPathName();
	Result.AssetName = NewAsset->GetName();
	return Result;
}

FApplyAnimCompressionResult FSkeletalMeshImplModule::ApplyAnimCompression(const FString& AnimSequencePath, const FString& CompressionSettingsPath)
{
	FApplyAnimCompressionResult Result;

	UAnimSequence* AnimSequence = LoadObject<UAnimSequence>(nullptr, *AnimSequencePath);
	if (!AnimSequence)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Animation sequence not found: %s"), *AnimSequencePath);
		return Result;
	}

	UAnimBoneCompressionSettings* BoneSettings = nullptr;
	if (!CompressionSettingsPath.IsEmpty())
	{
		BoneSettings = LoadObject<UAnimBoneCompressionSettings>(nullptr, *CompressionSettingsPath);
		if (!BoneSettings)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Compression settings not found: %s"), *CompressionSettingsPath);
			return Result;
		}
	}

	UAnimationBlueprintLibrary::SetBoneCompressionSettings(AnimSequence, BoneSettings);

	AnimSequence->PostEditChange();
	AnimSequence->MarkPackageDirty();

	Result.bSuccess = true;
	return Result;
}
