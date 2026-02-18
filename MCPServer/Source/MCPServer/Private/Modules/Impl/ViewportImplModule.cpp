// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/ViewportImplModule.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Editor.h"
#include "LevelEditorViewport.h"
#include "EditorViewportClient.h"
#include "LevelEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "UnrealClient.h"
#include "HighResScreenshot.h"
#include "Bookmarks/IBookmarkTypeTools.h"
#include "Engine/EngineBaseTypes.h"
#include "GameFramework/Actor.h"

FViewportImplModule::FViewportImplModule(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FViewportCameraResult FViewportImplModule::GetViewportCamera()
{
	FViewportCameraResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	Result.bSuccess = true;
	Result.Location = Client->GetViewLocation();
	Result.Rotation = Client->GetViewRotation();
	Result.FOV = Client->ViewFOV;
	return Result;
}

FSetViewportCameraResult FViewportImplModule::SetViewportCamera(const FVector& Location, const FRotator& Rotation)
{
	FSetViewportCameraResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	Client->SetViewLocation(Location);
	Client->SetViewRotation(Rotation);
	Client->Invalidate();

	Result.bSuccess = true;
	Result.Location = Client->GetViewLocation();
	Result.Rotation = Client->GetViewRotation();
	return Result;
}

FSimpleResult FViewportImplModule::SetGameView(bool bEnabled)
{
	FSimpleResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	LevelEditorSubsystem->EditorSetGameView(bEnabled);

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Game View %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
	return Result;
}

FSimpleResult FViewportImplModule::SetViewportRealtime(bool bEnabled)
{
	FSimpleResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	LevelEditorSubsystem->EditorSetViewportRealtime(bEnabled);

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Realtime rendering %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
	return Result;
}

FSimpleResult FViewportImplModule::SetViewMode(const FString& ModeName)
{
	FSimpleResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	EViewModeIndex ViewMode;
	if (ModeName.Equals(TEXT("Lit"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_Lit;
	}
	else if (ModeName.Equals(TEXT("Unlit"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_Unlit;
	}
	else if (ModeName.Equals(TEXT("Wireframe"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_Wireframe;
	}
	else if (ModeName.Equals(TEXT("BrushWireframe"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_BrushWireframe;
	}
	else if (ModeName.Equals(TEXT("LightingOnly"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_LightingOnly;
	}
	else if (ModeName.Equals(TEXT("LightComplexity"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_LightComplexity;
	}
	else if (ModeName.Equals(TEXT("ShaderComplexity"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_ShaderComplexity;
	}
	else if (ModeName.Equals(TEXT("DetailLighting"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_Lit_DetailLighting;
	}
	else if (ModeName.Equals(TEXT("CollisionPawn"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_CollisionPawn;
	}
	else if (ModeName.Equals(TEXT("CollisionVisibility"), ESearchCase::IgnoreCase))
	{
		ViewMode = VMI_CollisionVisibility;
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown view mode: %s. Supported: Lit, Unlit, Wireframe, BrushWireframe, LightingOnly, LightComplexity, ShaderComplexity, DetailLighting, CollisionPawn, CollisionVisibility"), *ModeName);
		return Result;
	}

	Client->SetViewMode(ViewMode);
	Client->Invalidate();

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("View mode set to %s"), *ModeName);
	return Result;
}

FSimpleResult FViewportImplModule::SetCameraFOV(float FOV)
{
	FSimpleResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	if (FOV < 5.0f || FOV > 170.0f)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("FOV must be between 5 and 170, got: %.1f"), FOV);
		return Result;
	}

	Client->ViewFOV = FOV;
	Client->FOVAngle = FOV;
	Client->Invalidate();

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Camera FOV set to %.1f"), FOV);
	return Result;
}

FSimpleResult FViewportImplModule::FocusOnActors(const TArray<FString>& ActorIdentifiers)
{
	FSimpleResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	FBox BoundingBox(ForceInit);
	int32 FoundCount = 0;

	TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();
	for (const FString& Identifier : ActorIdentifiers)
	{
		for (AActor* Actor : AllActors)
		{
			if (Actor && (Actor->GetName() == Identifier || Actor->GetActorLabel() == Identifier))
			{
				BoundingBox += Actor->GetComponentsBoundingBox(true);
				FoundCount++;
				break;
			}
		}
	}

	if (FoundCount == 0)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No actors found to focus on");
		return Result;
	}

	Client->FocusViewportOnBox(BoundingBox, true);

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Focused on %d actor(s)"), FoundCount);
	return Result;
}

FScreenshotResult FViewportImplModule::TakeScreenshot(const FString& Filename)
{
	FScreenshotResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	FString ScreenshotFilename = Filename;
	if (ScreenshotFilename.IsEmpty())
	{
		ScreenshotFilename = FPaths::ScreenShotDir() / TEXT("MCPScreenshot");
	}

	FScreenshotRequest::RequestScreenshot(ScreenshotFilename, false, true);

	Result.bSuccess = true;
	Result.FilePath = ScreenshotFilename;
	return Result;
}

FSimpleResult FViewportImplModule::PilotActor(const FString& ActorIdentifier)
{
	FSimpleResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	// Find the actor
	AActor* Actor = EditorActorSubsystem->GetActorReference(ActorIdentifier);
	if (!Actor)
	{
		TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();
		for (AActor* A : AllActors)
		{
			if (A && (A->GetName() == ActorIdentifier || A->GetActorLabel() == ActorIdentifier))
			{
				Actor = A;
				break;
			}
		}
	}

	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	LevelEditorSubsystem->PilotLevelActor(Actor);

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Now piloting actor: %s"), *Actor->GetActorLabel());
	return Result;
}

FSimpleResult FViewportImplModule::EjectPilot()
{
	FSimpleResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	LevelEditorSubsystem->EjectPilotLevelActor();

	Result.bSuccess = true;
	Result.Message = TEXT("Ejected from piloted actor");
	return Result;
}

FSimpleResult FViewportImplModule::SetBookmark(int32 Index)
{
	FSimpleResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	IBookmarkTypeTools::Get().CreateOrSetBookmark(Index, Client);

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Bookmark %d set"), Index);
	return Result;
}

FSimpleResult FViewportImplModule::JumpToBookmark(int32 Index)
{
	FSimpleResult Result;

	FLevelEditorViewportClient* Client = GCurrentLevelEditingViewportClient;
	if (!Client)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active viewport client");
		return Result;
	}

	if (!IBookmarkTypeTools::Get().CheckBookmark(Index, Client))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Bookmark %d does not exist"), Index);
		return Result;
	}

	IBookmarkTypeTools::Get().JumpToBookmark(Index, nullptr, Client);

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Jumped to bookmark %d"), Index);
	return Result;
}
