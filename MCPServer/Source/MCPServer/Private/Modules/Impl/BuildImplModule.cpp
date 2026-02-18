// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/BuildImplModule.h"
#include "EditorBuildUtils.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Editor.h"

FEditorBuildResult FBuildImplModule::BuildGeometry()
{
	FEditorBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	bool bSuccess = FEditorBuildUtils::EditorBuild(World, FBuildOptions::BuildGeometry, false);
	Result.bSuccess = bSuccess;
	if (!bSuccess)
	{
		Result.ErrorMessage = TEXT("Geometry build failed or was cancelled");
	}
	return Result;
}

FEditorBuildResult FBuildImplModule::BuildLighting()
{
	FEditorBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	bool bSuccess = FEditorBuildUtils::EditorBuild(World, FBuildOptions::BuildLighting, false);
	Result.bSuccess = bSuccess;
	if (!bSuccess)
	{
		Result.ErrorMessage = TEXT("Lighting build failed or was cancelled");
	}
	return Result;
}

FEditorBuildResult FBuildImplModule::BuildNavigationData()
{
	FEditorBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	bool bSuccess = FEditorBuildUtils::EditorBuild(World, FBuildOptions::BuildAIPaths, false);
	Result.bSuccess = bSuccess;
	if (!bSuccess)
	{
		Result.ErrorMessage = TEXT("Navigation data build failed or was cancelled");
	}
	return Result;
}

FEditorBuildResult FBuildImplModule::BuildHLOD()
{
	FEditorBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	bool bSuccess = FEditorBuildUtils::EditorBuild(World, FBuildOptions::BuildHierarchicalLOD, false);
	Result.bSuccess = bSuccess;
	if (!bSuccess)
	{
		Result.ErrorMessage = TEXT("HLOD build failed or was cancelled");
	}
	return Result;
}

FEditorBuildResult FBuildImplModule::BuildTextureStreaming()
{
	FEditorBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	bool bSuccess = FEditorBuildUtils::EditorBuildTextureStreaming(World);
	Result.bSuccess = bSuccess;
	if (!bSuccess)
	{
		Result.ErrorMessage = TEXT("Texture streaming build failed");
	}
	return Result;
}

FEditorBuildResult FBuildImplModule::BuildAll()
{
	FEditorBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	bool bSuccess = FEditorBuildUtils::EditorBuild(World, FBuildOptions::BuildAll, false);
	Result.bSuccess = bSuccess;
	if (!bSuccess)
	{
		Result.ErrorMessage = TEXT("Full build failed or was cancelled");
	}
	return Result;
}

FBuildStatusResult FBuildImplModule::GetBuildStatus()
{
	FBuildStatusResult Result;

	Result.bIsBuilding = FEditorBuildUtils::IsBuildCurrentlyRunning();
	Result.bIsLightingBuilding = GEditor->IsLightingBuildCurrentlyRunning();
	Result.bIsNavigationBuilding = false; // Navigation build status not easily queryable
	Result.bWasCancelled = GEditor->GetMapBuildCancelled();

	Result.bSuccess = true;
	return Result;
}
