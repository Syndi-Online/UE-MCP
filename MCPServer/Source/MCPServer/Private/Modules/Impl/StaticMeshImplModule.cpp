// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/StaticMeshImplModule.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshEditorSubsystem.h"
#include "PhysicsEngine/BodySetup.h"
#include "Materials/MaterialInterface.h"
#include "Editor.h"

FSetStaticMeshLodResult FStaticMeshImplModule::SetStaticMeshLod(const FString& MeshPath, const TArray<float>& ScreenSizes)
{
	FSetStaticMeshLodResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	bool bSet = Subsystem->SetLodScreenSizes(Mesh, ScreenSizes);
	if (!bSet)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to set LOD screen sizes");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FImportLodResult FStaticMeshImplModule::ImportLod(const FString& MeshPath, int32 LodIndex, const FString& SourceFilePath)
{
	FImportLodResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	int32 ImportedIndex = Subsystem->ImportLOD(Mesh, LodIndex, SourceFilePath);
	if (ImportedIndex == INDEX_NONE)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to import LOD %d from: %s"), LodIndex, *SourceFilePath);
		return Result;
	}

	Result.bSuccess = true;
	Result.LodIndex = ImportedIndex;
	return Result;
}

FGetLodSettingsResult FStaticMeshImplModule::GetLodSettings(const FString& MeshPath)
{
	FGetLodSettingsResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	Result.LodCount = Subsystem->GetLodCount(Mesh);
	Result.LodGroup = Subsystem->GetLODGroup(Mesh).ToString();

	TArray<float> ScreenSizes = Subsystem->GetLodScreenSizes(Mesh);

	for (int32 i = 0; i < Result.LodCount; ++i)
	{
		FLodInfo Info;
		Info.LodIndex = i;
		Info.ScreenSize = ScreenSizes.IsValidIndex(i) ? ScreenSizes[i] : 0.0f;
		Info.NumVertices = Subsystem->GetNumberVerts(Mesh, i);

		if (Mesh->GetRenderData() && Mesh->GetRenderData()->LODResources.IsValidIndex(i))
		{
			Info.NumTriangles = Mesh->GetRenderData()->LODResources[i].GetNumTriangles();
		}

		Result.Lods.Add(Info);
	}

	Result.bSuccess = true;
	return Result;
}

FSetCollisionResult FStaticMeshImplModule::SetCollision(const FString& MeshPath, const FString& CollisionType)
{
	FSetCollisionResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	if (CollisionType.Equals(TEXT("Box"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::Box);
	}
	else if (CollisionType.Equals(TEXT("Sphere"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::Sphere);
	}
	else if (CollisionType.Equals(TEXT("Capsule"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::Capsule);
	}
	else if (CollisionType.Equals(TEXT("NDOP10_X"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::NDOP10_X);
	}
	else if (CollisionType.Equals(TEXT("NDOP10_Y"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::NDOP10_Y);
	}
	else if (CollisionType.Equals(TEXT("NDOP10_Z"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::NDOP10_Z);
	}
	else if (CollisionType.Equals(TEXT("NDOP18"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::NDOP18);
	}
	else if (CollisionType.Equals(TEXT("NDOP26"), ESearchCase::IgnoreCase))
	{
		Subsystem->AddSimpleCollisions(Mesh, EScriptCollisionShapeType::NDOP26);
	}
	else if (CollisionType.Equals(TEXT("ComplexAsSimple"), ESearchCase::IgnoreCase))
	{
		UBodySetup* BodySetup = Mesh->GetBodySetup();
		if (BodySetup)
		{
			BodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
			Mesh->PostEditChange();
			Mesh->MarkPackageDirty();
		}
	}
	else if (CollisionType.Equals(TEXT("UseSimpleAsComplex"), ESearchCase::IgnoreCase))
	{
		UBodySetup* BodySetup = Mesh->GetBodySetup();
		if (BodySetup)
		{
			BodySetup->CollisionTraceFlag = CTF_UseSimpleAsComplex;
			Mesh->PostEditChange();
			Mesh->MarkPackageDirty();
		}
	}
	else if (CollisionType.Equals(TEXT("RemoveAll"), ESearchCase::IgnoreCase))
	{
		Subsystem->RemoveCollisions(Mesh);
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown collision type: %s. Supported: Box, Sphere, Capsule, NDOP10_X, NDOP10_Y, NDOP10_Z, NDOP18, NDOP26, ComplexAsSimple, UseSimpleAsComplex, RemoveAll"), *CollisionType);
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FUVChannelResult FStaticMeshImplModule::AddUVChannel(const FString& MeshPath, int32 LodIndex)
{
	FUVChannelResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	bool bAdded = Subsystem->AddUVChannel(Mesh, LodIndex);
	if (!bAdded)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add UV channel at LOD %d"), LodIndex);
		return Result;
	}

	Result.bSuccess = true;
	Result.NumUVChannels = Subsystem->GetNumUVChannels(Mesh, LodIndex);
	return Result;
}

FUVChannelResult FStaticMeshImplModule::RemoveUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex)
{
	FUVChannelResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	bool bRemoved = Subsystem->RemoveUVChannel(Mesh, LodIndex, UVChannelIndex);
	if (!bRemoved)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to remove UV channel %d at LOD %d"), UVChannelIndex, LodIndex);
		return Result;
	}

	Result.bSuccess = true;
	Result.NumUVChannels = Subsystem->GetNumUVChannels(Mesh, LodIndex);
	return Result;
}

FUVChannelResult FStaticMeshImplModule::GenerateUVChannel(const FString& MeshPath, int32 LodIndex, int32 UVChannelIndex, const FString& ProjectionType)
{
	FUVChannelResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UStaticMeshEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
	if (!Subsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UStaticMeshEditorSubsystem is not available");
		return Result;
	}

	bool bGenerated = false;
	FVector Position = FVector::ZeroVector;
	FRotator Orientation = FRotator::ZeroRotator;

	if (ProjectionType.Equals(TEXT("Planar"), ESearchCase::IgnoreCase))
	{
		bGenerated = Subsystem->GeneratePlanarUVChannel(Mesh, LodIndex, UVChannelIndex, Position, Orientation, FVector2D(1.0f, 1.0f));
	}
	else if (ProjectionType.Equals(TEXT("Cylindrical"), ESearchCase::IgnoreCase))
	{
		bGenerated = Subsystem->GenerateCylindricalUVChannel(Mesh, LodIndex, UVChannelIndex, Position, Orientation, FVector2D(1.0f, 1.0f));
	}
	else if (ProjectionType.Equals(TEXT("Box"), ESearchCase::IgnoreCase))
	{
		bGenerated = Subsystem->GenerateBoxUVChannel(Mesh, LodIndex, UVChannelIndex, Position, Orientation, FVector(1.0f, 1.0f, 1.0f));
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown projection type: %s. Supported: Planar, Cylindrical, Box"), *ProjectionType);
		return Result;
	}

	if (!bGenerated)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to generate %s UV at LOD %d, channel %d"), *ProjectionType, LodIndex, UVChannelIndex);
		return Result;
	}

	Result.bSuccess = true;
	Result.NumUVChannels = Subsystem->GetNumUVChannels(Mesh, LodIndex);
	return Result;
}

FSetMeshMaterialResult FStaticMeshImplModule::SetMeshMaterial(const FString& MeshPath, int32 MaterialIndex, const FString& MaterialPath)
{
	FSetMeshMaterialResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, *MaterialPath);
	if (!Material)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Material not found: %s"), *MaterialPath);
		return Result;
	}

	if (MaterialIndex < 0 || MaterialIndex >= Mesh->GetStaticMaterials().Num())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Invalid material index: %d (mesh has %d material slots)"), MaterialIndex, Mesh->GetStaticMaterials().Num());
		return Result;
	}

	Mesh->SetMaterial(MaterialIndex, Material);
	Mesh->PostEditChange();
	Mesh->MarkPackageDirty();

	Result.bSuccess = true;
	return Result;
}

FMeshBoundsResult FStaticMeshImplModule::GetMeshBounds(const FString& MeshPath)
{
	FMeshBoundsResult Result;

	UStaticMesh* Mesh = LoadObject<UStaticMesh>(nullptr, *MeshPath);
	if (!Mesh)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Static mesh not found: %s"), *MeshPath);
		return Result;
	}

	FBoxSphereBounds Bounds = Mesh->GetBounds();
	Result.bSuccess = true;
	Result.Origin = Bounds.Origin;
	Result.BoxExtent = Bounds.BoxExtent;
	Result.SphereRadius = Bounds.SphereRadius;
	return Result;
}
