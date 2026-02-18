// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ILevelModule.h"
#include "MockCallRecorder.h"

class FMockLevelModule : public ILevelModule
{
public:
	FMockCallRecorder Recorder;

	FNewLevelResult NewLevelResult;
	FNewLevelResult NewLevelFromTemplateResult;
	FLoadLevelResult LoadLevelResult;
	FSaveLevelResult SaveLevelResult;
	FSaveAllDirtyLevelsResult SaveAllDirtyLevelsResult;
	FAddSublevelResult AddSublevelResult;
	FRemoveSublevelResult RemoveSublevelResult;
	FSetCurrentLevelResult SetCurrentLevelResult;
	FSetLevelVisibilityResult SetLevelVisibilityResult;

	virtual FNewLevelResult NewLevel(const FString& LevelPath) override
	{
		Recorder.RecordCall(TEXT("NewLevel"));
		return NewLevelResult;
	}

	virtual FNewLevelResult NewLevelFromTemplate(const FString& LevelPath, const FString& TemplatePath) override
	{
		Recorder.RecordCall(TEXT("NewLevelFromTemplate"));
		return NewLevelFromTemplateResult;
	}

	virtual FLoadLevelResult LoadLevel(const FString& LevelPath) override
	{
		Recorder.RecordCall(TEXT("LoadLevel"));
		return LoadLevelResult;
	}

	virtual FSaveLevelResult SaveLevel() override
	{
		Recorder.RecordCall(TEXT("SaveLevel"));
		return SaveLevelResult;
	}

	virtual FSaveAllDirtyLevelsResult SaveAllDirtyLevels() override
	{
		Recorder.RecordCall(TEXT("SaveAllDirtyLevels"));
		return SaveAllDirtyLevelsResult;
	}

	virtual FAddSublevelResult AddSublevel(const FString& LevelPath) override
	{
		Recorder.RecordCall(TEXT("AddSublevel"));
		return AddSublevelResult;
	}

	virtual FRemoveSublevelResult RemoveSublevel(const FString& LevelName) override
	{
		Recorder.RecordCall(TEXT("RemoveSublevel"));
		return RemoveSublevelResult;
	}

	virtual FSetCurrentLevelResult SetCurrentLevel(const FString& LevelName) override
	{
		Recorder.RecordCall(TEXT("SetCurrentLevel"));
		return SetCurrentLevelResult;
	}

	virtual FSetLevelVisibilityResult SetLevelVisibility(const FString& LevelName, bool bVisible) override
	{
		Recorder.RecordCall(TEXT("SetLevelVisibility"));
		return SetLevelVisibilityResult;
	}
};
