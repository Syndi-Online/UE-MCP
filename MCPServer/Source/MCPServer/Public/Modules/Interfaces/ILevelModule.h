// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

/**
 * Result of creating a new level.
 */
struct FNewLevelResult
{
	bool bSuccess = false;
	FString LevelName;
	FString LevelPath;
	FString ErrorMessage;
};

/**
 * Result of loading a level.
 */
struct FLoadLevelResult
{
	bool bSuccess = false;
	FString LevelName;
	FString ErrorMessage;
};

/**
 * Result of saving a level.
 */
struct FSaveLevelResult
{
	bool bSuccess = false;
	FString LevelName;
	FString ErrorMessage;
};

/**
 * Result of saving all dirty levels.
 */
struct FSaveAllDirtyLevelsResult
{
	bool bSuccess = false;
	int32 SavedCount = 0;
	FString ErrorMessage;
};

/**
 * Result of adding a sublevel.
 */
struct FAddSublevelResult
{
	bool bSuccess = false;
	FString SublevelName;
	FString ErrorMessage;
};

/**
 * Result of removing a sublevel.
 */
struct FRemoveSublevelResult
{
	bool bSuccess = false;
	FString SublevelName;
	FString ErrorMessage;
};

/**
 * Result of setting the current level.
 */
struct FSetCurrentLevelResult
{
	bool bSuccess = false;
	FString LevelName;
	FString ErrorMessage;
};

/**
 * Result of setting level visibility.
 */
struct FSetLevelVisibilityResult
{
	bool bSuccess = false;
	FString LevelName;
	bool bVisible = false;
	FString ErrorMessage;
};

/**
 * Module interface for level management operations in the editor.
 */
class ILevelModule
{
public:
	virtual ~ILevelModule() = default;

	/** Create a new empty level. */
	virtual FNewLevelResult NewLevel(const FString& LevelPath) = 0;

	/** Create a new level from a template. */
	virtual FNewLevelResult NewLevelFromTemplate(const FString& LevelPath, const FString& TemplatePath) = 0;

	/** Load/open an existing level. */
	virtual FLoadLevelResult LoadLevel(const FString& LevelPath) = 0;

	/** Save the current level. */
	virtual FSaveLevelResult SaveLevel() = 0;

	/** Save all dirty (modified) levels. */
	virtual FSaveAllDirtyLevelsResult SaveAllDirtyLevels() = 0;

	/** Add a streaming sublevel to the current world. */
	virtual FAddSublevelResult AddSublevel(const FString& LevelPath) = 0;

	/** Remove a streaming sublevel from the current world. */
	virtual FRemoveSublevelResult RemoveSublevel(const FString& LevelName) = 0;

	/** Set the current working level for actor placement. */
	virtual FSetCurrentLevelResult SetCurrentLevel(const FString& LevelName) = 0;

	/** Set visibility of a streaming level. */
	virtual FSetLevelVisibilityResult SetLevelVisibility(const FString& LevelName, bool bVisible) = 0;
};
