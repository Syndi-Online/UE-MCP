// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IProjectSettingsModule.h"

class FProjectSettingsImplModule : public IProjectSettingsModule
{
public:
	virtual FProjectInfoResult GetProjectInfo() override;
	virtual FConfigValueResult GetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key) override;
	virtual FConfigValueResult SetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key, const FString& Value) override;
	virtual FProjectPathsResult GetProjectPaths() override;

private:
	FString ResolveConfigFilename(const FString& ConfigName) const;
};
