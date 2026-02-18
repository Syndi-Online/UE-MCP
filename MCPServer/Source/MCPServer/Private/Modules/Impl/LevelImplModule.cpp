// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/LevelImplModule.h"
#include "Editor.h"
#include "LevelEditorSubsystem.h"
#include "EditorLevelUtils.h"
#include "Engine/World.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "FileHelpers.h"

FNewLevelResult FLevelImplModule::NewLevel(const FString& LevelPath)
{
	FNewLevelResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	bool bCreated = LevelEditorSubsystem->NewLevel(LevelPath);
	if (!bCreated)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create new level: %s"), *LevelPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.LevelPath = LevelPath;
	Result.LevelName = FPaths::GetBaseFilename(LevelPath);
	return Result;
}

FNewLevelResult FLevelImplModule::NewLevelFromTemplate(const FString& LevelPath, const FString& TemplatePath)
{
	FNewLevelResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	bool bCreated = LevelEditorSubsystem->NewLevelFromTemplate(LevelPath, TemplatePath);
	if (!bCreated)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create level from template. Path: %s, Template: %s"),
			*LevelPath, *TemplatePath);
		return Result;
	}

	Result.bSuccess = true;
	Result.LevelPath = LevelPath;
	Result.LevelName = FPaths::GetBaseFilename(LevelPath);
	return Result;
}

FLoadLevelResult FLevelImplModule::LoadLevel(const FString& LevelPath)
{
	FLoadLevelResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	bool bLoaded = LevelEditorSubsystem->LoadLevel(LevelPath);
	if (!bLoaded)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to load level: %s"), *LevelPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.LevelName = FPaths::GetBaseFilename(LevelPath);
	return Result;
}

FSaveLevelResult FLevelImplModule::SaveLevel()
{
	FSaveLevelResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	bool bSaved = LevelEditorSubsystem->SaveCurrentLevel();
	if (!bSaved)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to save current level");
		return Result;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	Result.bSuccess = true;
	Result.LevelName = World ? World->GetMapName() : TEXT("Unknown");
	return Result;
}

FSaveAllDirtyLevelsResult FLevelImplModule::SaveAllDirtyLevels()
{
	FSaveAllDirtyLevelsResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	bool bSaved = LevelEditorSubsystem->SaveAllDirtyLevels();
	if (!bSaved)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to save all dirty levels");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FAddSublevelResult FLevelImplModule::AddSublevel(const FString& LevelPath)
{
	FAddSublevelResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	ULevelStreaming* StreamingLevel = UEditorLevelUtils::AddLevelToWorld(
		World, *LevelPath, ULevelStreamingDynamic::StaticClass());

	if (!StreamingLevel)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add sublevel: %s"), *LevelPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.SublevelName = StreamingLevel->GetWorldAssetPackageFName().ToString();
	return Result;
}

FRemoveSublevelResult FLevelImplModule::RemoveSublevel(const FString& LevelName)
{
	FRemoveSublevelResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (!StreamingLevel) continue;

		FString PackageName = StreamingLevel->GetWorldAssetPackageFName().ToString();
		if (PackageName.Contains(LevelName))
		{
			ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();
			if (LoadedLevel)
			{
				bool bRemoved = UEditorLevelUtils::RemoveLevelFromWorld(LoadedLevel);
				if (bRemoved)
				{
					Result.bSuccess = true;
					Result.SublevelName = PackageName;
					return Result;
				}
				else
				{
					Result.bSuccess = false;
					Result.ErrorMessage = FString::Printf(TEXT("Failed to remove sublevel: %s"), *LevelName);
					return Result;
				}
			}
			else
			{
				// Level not loaded, remove the invalid streaming reference
				UEditorLevelUtils::RemoveInvalidLevelFromWorld(StreamingLevel);
				Result.bSuccess = true;
				Result.SublevelName = PackageName;
				return Result;
			}
		}
	}

	Result.bSuccess = false;
	Result.ErrorMessage = FString::Printf(TEXT("Sublevel not found: %s"), *LevelName);
	return Result;
}

FSetCurrentLevelResult FLevelImplModule::SetCurrentLevel(const FString& LevelName)
{
	FSetCurrentLevelResult Result;

	ULevelEditorSubsystem* LevelEditorSubsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	if (!LevelEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("ULevelEditorSubsystem is not available");
		return Result;
	}

	bool bSet = LevelEditorSubsystem->SetCurrentLevelByName(FName(*LevelName));
	if (!bSet)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to set current level: %s"), *LevelName);
		return Result;
	}

	Result.bSuccess = true;
	Result.LevelName = LevelName;
	return Result;
}

FSetLevelVisibilityResult FLevelImplModule::SetLevelVisibility(const FString& LevelName, bool bVisible)
{
	FSetLevelVisibilityResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	// Check persistent level
	if (LevelName.Equals(TEXT("PersistentLevel"), ESearchCase::IgnoreCase) && World->PersistentLevel)
	{
		UEditorLevelUtils::SetLevelVisibility(World->PersistentLevel, bVisible, false);
		Result.bSuccess = true;
		Result.LevelName = TEXT("PersistentLevel");
		Result.bVisible = bVisible;
		return Result;
	}

	// Search streaming levels
	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (!StreamingLevel) continue;

		FString PackageName = StreamingLevel->GetWorldAssetPackageFName().ToString();
		if (PackageName.Contains(LevelName))
		{
			ULevel* LoadedLevel = StreamingLevel->GetLoadedLevel();
			if (LoadedLevel)
			{
				UEditorLevelUtils::SetLevelVisibility(LoadedLevel, bVisible, false);
				Result.bSuccess = true;
				Result.LevelName = PackageName;
				Result.bVisible = bVisible;
				return Result;
			}
			else
			{
				Result.bSuccess = false;
				Result.ErrorMessage = FString::Printf(TEXT("Level '%s' is not loaded"), *LevelName);
				return Result;
			}
		}
	}

	Result.bSuccess = false;
	Result.ErrorMessage = FString::Printf(TEXT("Level not found: %s"), *LevelName);
	return Result;
}
