// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/FoliageImplModule.h"
#include "InstancedFoliageActor.h"
#include "FoliageType_InstancedStaticMesh.h"
#include "ProceduralFoliageVolume.h"
#include "ProceduralFoliageComponent.h"
#include "EngineUtils.h"
#include "Editor.h"
#include "Engine/StaticMesh.h"

FFoliageAddInstancesResult FFoliageImplModule::AddFoliageInstances(const FString& MeshPath, const TArray<FTransform>& Transforms)
{
	FFoliageAddInstancesResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AInstancedFoliageActor* IFA = AInstancedFoliageActor::Get(World, true);
	if (!IFA)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to get or create InstancedFoliageActor");
		return Result;
	}

	UFoliageType* FoliageType = nullptr;
	FFoliageInfo* FoliageInfo = IFA->AddMesh(Mesh, &FoliageType);
	if (!FoliageInfo || !FoliageType)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to register mesh as foliage type");
		return Result;
	}

	for (const FTransform& T : Transforms)
	{
		FFoliageInstance Instance;
		Instance.SetInstanceWorldTransform(T);
		FoliageInfo->AddInstance(FoliageType, Instance);
	}

	FoliageInfo->Refresh(true, false);

	Result.bSuccess = true;
	Result.InstancesAdded = Transforms.Num();
	return Result;
}

FFoliageRemoveInstancesResult FFoliageImplModule::RemoveFoliageInstances(const FString& MeshPath, const FVector& Center, float Radius)
{
	FFoliageRemoveInstancesResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AInstancedFoliageActor* IFA = AInstancedFoliageActor::Get(World, false);
	if (!IFA)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No InstancedFoliageActor found in level");
		return Result;
	}

	int32 TotalRemoved = 0;

	UStaticMesh* TargetMesh = nullptr;
	if (!MeshPath.IsEmpty())
	{
		TargetMesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
		if (!TargetMesh)
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
			return Result;
		}
	}

	IFA->ForEachFoliageInfo([&](UFoliageType* FoliageType, FFoliageInfo& Info) -> bool
	{
		// If a specific mesh was requested, filter by it
		if (TargetMesh)
		{
			UFoliageType_InstancedStaticMesh* ISMType = Cast<UFoliageType_InstancedStaticMesh>(FoliageType);
			if (!ISMType || ISMType->GetStaticMesh() != TargetMesh)
			{
				return true; // continue
			}
		}

		if (Radius <= 0.0f && TargetMesh)
		{
			// Remove all instances of this type
			int32 Count = Info.Instances.Num();
			TArray<int32> AllIndices;
			AllIndices.Reserve(Count);
			for (int32 i = 0; i < Count; ++i)
			{
				AllIndices.Add(i);
			}
			if (AllIndices.Num() > 0)
			{
				TotalRemoved += AllIndices.Num();
				Info.RemoveInstances(AllIndices, true);
			}
		}
		else
		{
			TArray<int32> InstancesToRemove;
			Info.GetInstancesInsideSphere(FSphere(Center, Radius), InstancesToRemove);
			if (InstancesToRemove.Num() > 0)
			{
				TotalRemoved += InstancesToRemove.Num();
				Info.RemoveInstances(InstancesToRemove, true);
			}
		}

		// If filtering by mesh, stop after finding it
		return TargetMesh == nullptr;
	});

	Result.bSuccess = true;
	Result.InstancesRemoved = TotalRemoved;
	return Result;
}

FFoliageRegisterMeshResult FFoliageImplModule::RegisterFoliageMesh(const FString& MeshPath)
{
	FFoliageRegisterMeshResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AInstancedFoliageActor* IFA = AInstancedFoliageActor::Get(World, true);
	if (!IFA)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to get or create InstancedFoliageActor");
		return Result;
	}

	UFoliageType* FoliageType = nullptr;
	FFoliageInfo* FoliageInfo = IFA->AddMesh(Mesh, &FoliageType);
	if (!FoliageInfo || !FoliageType)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to register mesh as foliage type");
		return Result;
	}

	Result.bSuccess = true;
	Result.FoliageTypeName = FoliageType->GetName();
	return Result;
}

FFoliageResimulateResult FFoliageImplModule::ResimulateProceduralFoliage(const FString& VolumeName)
{
	FFoliageResimulateResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	TArray<AProceduralFoliageVolume*> Volumes;

	if (VolumeName.IsEmpty())
	{
		// Collect all procedural foliage volumes
		for (TActorIterator<AProceduralFoliageVolume> It(World); It; ++It)
		{
			Volumes.Add(*It);
		}
	}
	else
	{
		// Find specific volume by name
		for (TActorIterator<AProceduralFoliageVolume> It(World); It; ++It)
		{
			if (It->GetActorLabel().Equals(VolumeName, ESearchCase::IgnoreCase) ||
				It->GetName().Equals(VolumeName, ESearchCase::IgnoreCase))
			{
				Volumes.Add(*It);
				break;
			}
		}
	}

	if (Volumes.Num() == 0)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = VolumeName.IsEmpty()
			? TEXT("No procedural foliage volumes found in level")
			: FString::Printf(TEXT("Procedural foliage volume not found: %s"), *VolumeName);
		return Result;
	}

	// Call via reflection since the function is a UFUNCTION but not DLL-exported
	UClass* LibClass = FindObject<UClass>(nullptr, TEXT("/Script/FoliageEdit.ProceduralFoliageEditorLibrary"));
	if (LibClass)
	{
		UFunction* Func = LibClass->FindFunctionByName(TEXT("ResimulateProceduralFoliageVolumes"));
		if (Func)
		{
			struct { TArray<AProceduralFoliageVolume*> InVolumes; } Params;
			Params.InVolumes = Volumes;
			LibClass->GetDefaultObject()->ProcessEvent(Func, &Params);
		}
	}

	Result.bSuccess = true;
	Result.VolumesProcessed = Volumes.Num();
	return Result;
}
