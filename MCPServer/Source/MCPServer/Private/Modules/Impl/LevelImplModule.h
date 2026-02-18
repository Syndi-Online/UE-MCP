// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ILevelModule.h"

/**
 * Level module implementation using ULevelEditorSubsystem and UEditorLevelUtils.
 */
class FLevelImplModule : public ILevelModule
{
public:
	virtual FNewLevelResult NewLevel(const FString& LevelPath) override;
	virtual FNewLevelResult NewLevelFromTemplate(const FString& LevelPath, const FString& TemplatePath) override;
	virtual FLoadLevelResult LoadLevel(const FString& LevelPath) override;
	virtual FSaveLevelResult SaveLevel() override;
	virtual FSaveAllDirtyLevelsResult SaveAllDirtyLevels() override;
	virtual FAddSublevelResult AddSublevel(const FString& LevelPath) override;
	virtual FRemoveSublevelResult RemoveSublevel(const FString& LevelName) override;
	virtual FSetCurrentLevelResult SetCurrentLevel(const FString& LevelName) override;
	virtual FSetLevelVisibilityResult SetLevelVisibility(const FString& LevelName, bool bVisible) override;
};
