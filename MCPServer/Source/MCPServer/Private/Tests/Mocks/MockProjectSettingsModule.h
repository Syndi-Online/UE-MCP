// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IProjectSettingsModule.h"
#include "MockCallRecorder.h"

class FMockProjectSettingsModule : public IProjectSettingsModule
{
public:
	FMockCallRecorder Recorder;

	FProjectInfoResult GetProjectInfoResult;
	FConfigValueResult GetConfigValueResult;
	FConfigValueResult SetConfigValueResult;
	FProjectPathsResult GetProjectPathsResult;

	virtual FProjectInfoResult GetProjectInfo() override { Recorder.RecordCall(TEXT("GetProjectInfo")); return GetProjectInfoResult; }
	virtual FConfigValueResult GetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key) override { Recorder.RecordCall(TEXT("GetConfigValue")); return GetConfigValueResult; }
	virtual FConfigValueResult SetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key, const FString& Value) override { Recorder.RecordCall(TEXT("SetConfigValue")); return SetConfigValueResult; }
	virtual FProjectPathsResult GetProjectPaths() override { Recorder.RecordCall(TEXT("GetProjectPaths")); return GetProjectPathsResult; }
};
