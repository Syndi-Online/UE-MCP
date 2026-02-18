// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FProjectInfoResult
{
	bool bSuccess = false;
	FString ProjectName;
	FString ProjectVersion;
	FString Description;
	FString CompanyName;
	FString EngineVersion;
	FString ProjectFilePath;
	FString ErrorMessage;
};

struct FConfigValueResult
{
	bool bSuccess = false;
	FString Section;
	FString Key;
	FString Value;
	FString ErrorMessage;
};

struct FProjectPathsResult
{
	bool bSuccess = false;
	FString ProjectDir;
	FString ContentDir;
	FString ConfigDir;
	FString SavedDir;
	FString PluginsDir;
	FString IntermediateDir;
	FString LogDir;
	FString ProjectFilePath;
	FString EngineDir;
	FString ErrorMessage;
};

/**
 * Module interface for Project Settings operations.
 */
class IProjectSettingsModule
{
public:
	virtual ~IProjectSettingsModule() = default;

	virtual FProjectInfoResult GetProjectInfo() = 0;
	virtual FConfigValueResult GetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key) = 0;
	virtual FConfigValueResult SetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key, const FString& Value) = 0;
	virtual FProjectPathsResult GetProjectPaths() = 0;
};
