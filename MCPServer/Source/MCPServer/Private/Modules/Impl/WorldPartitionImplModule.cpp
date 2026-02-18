// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/WorldPartitionImplModule.h"
#include "Editor.h"
#include "WorldPartition/IWorldPartitionEditorModule.h"
#include "WorldPartition/WorldPartition.h"

static UWorld* GetEditorWorld()
{
	if (GEditor)
	{
		return GEditor->GetEditorWorldContext().World();
	}
	return nullptr;
}

FWorldPartitionResult FWorldPartitionImplModule::BuildHLOD()
{
	FWorldPartitionResult Result;

	UWorld* World = GetEditorWorld();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	if (!World->IsPartitionedWorld())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Current world is not a World Partition world");
		return Result;
	}

	UClass* HLODBuilderClass = FindObject<UClass>(nullptr, TEXT("/Script/UnrealEd.WorldPartitionHLODsBuilder"));
	if (!HLODBuilderClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("WorldPartitionHLODsBuilder class not found");
		return Result;
	}

	IWorldPartitionEditorModule& WPEditorModule = IWorldPartitionEditorModule::Get();

	IWorldPartitionEditorModule::FRunBuilderParams Params;
	Params.BuilderClass = HLODBuilderClass;
	Params.World = World;
	Params.ExtraArgs = TEXT("-SetupHLODs -BuildHLODs");
	Params.OperationDescription = FText::FromString(TEXT("Building HLODs..."));

	bool bBuilt = WPEditorModule.RunBuilder(Params);
	Result.bSuccess = bBuilt;
	Result.Message = bBuilt ? TEXT("HLOD build completed") : TEXT("HLOD build failed");
	if (!bBuilt)
	{
		Result.ErrorMessage = TEXT("HLOD build failed");
	}
	return Result;
}

FWorldPartitionResult FWorldPartitionImplModule::DeleteHLOD()
{
	FWorldPartitionResult Result;

	UWorld* World = GetEditorWorld();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	if (!World->IsPartitionedWorld())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Current world is not a World Partition world");
		return Result;
	}

	UClass* HLODBuilderClass = FindObject<UClass>(nullptr, TEXT("/Script/UnrealEd.WorldPartitionHLODsBuilder"));
	if (!HLODBuilderClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("WorldPartitionHLODsBuilder class not found");
		return Result;
	}

	IWorldPartitionEditorModule& WPEditorModule = IWorldPartitionEditorModule::Get();

	IWorldPartitionEditorModule::FRunBuilderParams Params;
	Params.BuilderClass = HLODBuilderClass;
	Params.World = World;
	Params.ExtraArgs = TEXT("-DeleteHLODs");
	Params.OperationDescription = FText::FromString(TEXT("Deleting HLODs..."));

	bool bDeleted = WPEditorModule.RunBuilder(Params);
	Result.bSuccess = bDeleted;
	Result.Message = bDeleted ? TEXT("HLOD data deleted") : TEXT("HLOD deletion failed");
	if (!bDeleted)
	{
		Result.ErrorMessage = TEXT("HLOD deletion failed");
	}
	return Result;
}
