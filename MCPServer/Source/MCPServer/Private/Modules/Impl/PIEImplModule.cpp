// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/PIEImplModule.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "LevelEditor.h"
#include "IAssetViewport.h"

FPIEResult FPIEImplModule::PlayInEditor()
{
	FPIEResult Result;

	if (!GUnrealEd)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Editor engine not available");
		return Result;
	}

	if (GUnrealEd->IsPlaySessionInProgress())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("A play session is already in progress");
		return Result;
	}

	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<IAssetViewport> ActiveViewport = LevelEditorModule.GetFirstActiveViewport();

	FRequestPlaySessionParams SessionParams;
	SessionParams.WorldType = EPlaySessionWorldType::PlayInEditor;
	if (ActiveViewport.IsValid())
	{
		SessionParams.DestinationSlateViewport = ActiveViewport;
	}

	GUnrealEd->RequestPlaySession(SessionParams);

	Result.bSuccess = true;
	Result.Message = TEXT("Play In Editor session requested");
	return Result;
}

FPIEResult FPIEImplModule::StopPIE()
{
	FPIEResult Result;

	if (!GUnrealEd)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Editor engine not available");
		return Result;
	}

	if (!GUnrealEd->IsPlayingSessionInEditor())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No play session is currently active");
		return Result;
	}

	GUnrealEd->RequestEndPlayMap();

	Result.bSuccess = true;
	Result.Message = TEXT("Play session end requested");
	return Result;
}

FPIEResult FPIEImplModule::SimulateInEditor()
{
	FPIEResult Result;

	if (!GUnrealEd)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Editor engine not available");
		return Result;
	}

	if (GUnrealEd->IsPlaySessionInProgress())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("A play session is already in progress");
		return Result;
	}

	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<IAssetViewport> ActiveViewport = LevelEditorModule.GetFirstActiveViewport();

	FRequestPlaySessionParams SessionParams;
	SessionParams.WorldType = EPlaySessionWorldType::SimulateInEditor;
	if (ActiveViewport.IsValid())
	{
		SessionParams.DestinationSlateViewport = ActiveViewport;
	}

	GUnrealEd->RequestPlaySession(SessionParams);

	Result.bSuccess = true;
	Result.Message = TEXT("Simulate In Editor session requested");
	return Result;
}

FPIEStatusResult FPIEImplModule::IsPlaying()
{
	FPIEStatusResult Result;

	if (!GUnrealEd)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Editor engine not available");
		return Result;
	}

	Result.bSuccess = true;
	Result.bIsPlaying = GUnrealEd->IsPlayingSessionInEditor();
	Result.bIsSimulating = GUnrealEd->IsSimulatingInEditor();
	return Result;
}
