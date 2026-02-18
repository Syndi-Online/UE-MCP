// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/ProjectSettingsImplModule.h"
#include "Misc/App.h"
#include "Misc/Paths.h"
#include "Misc/EngineVersion.h"
#include "Misc/ConfigCacheIni.h"
#include "GeneralProjectSettings.h"

FProjectInfoResult FProjectSettingsImplModule::GetProjectInfo()
{
	FProjectInfoResult Result;

	const UGeneralProjectSettings* Settings = GetDefault<UGeneralProjectSettings>();
	if (!Settings)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to get project settings");
		return Result;
	}

	Result.bSuccess = true;
	Result.ProjectName = Settings->ProjectName.IsEmpty() ? FString(FApp::GetProjectName()) : Settings->ProjectName;
	Result.ProjectVersion = Settings->ProjectVersion;
	Result.Description = Settings->Description;
	Result.CompanyName = Settings->CompanyName;
	Result.EngineVersion = FEngineVersion::Current().ToString(EVersionComponent::Patch);
	Result.ProjectFilePath = FPaths::GetProjectFilePath();
	return Result;
}

FConfigValueResult FProjectSettingsImplModule::GetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key)
{
	FConfigValueResult Result;

	FString Filename = ResolveConfigFilename(ConfigName);
	if (Filename.IsEmpty())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown config name: %s"), *ConfigName);
		return Result;
	}

	FString Value;
	if (!GConfig->GetString(*Section, *Key, Value, Filename))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Key not found: [%s] %s in %s"), *Section, *Key, *ConfigName);
		return Result;
	}

	Result.bSuccess = true;
	Result.Section = Section;
	Result.Key = Key;
	Result.Value = Value;
	return Result;
}

FConfigValueResult FProjectSettingsImplModule::SetConfigValue(const FString& ConfigName, const FString& Section, const FString& Key, const FString& Value)
{
	FConfigValueResult Result;

	FString Filename = ResolveConfigFilename(ConfigName);
	if (Filename.IsEmpty())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown config name: %s"), *ConfigName);
		return Result;
	}

	GConfig->SetString(*Section, *Key, *Value, Filename);
	GConfig->Flush(false, Filename);

	Result.bSuccess = true;
	Result.Section = Section;
	Result.Key = Key;
	Result.Value = Value;
	return Result;
}

FProjectPathsResult FProjectSettingsImplModule::GetProjectPaths()
{
	FProjectPathsResult Result;
	Result.bSuccess = true;
	Result.ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	Result.ContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	Result.ConfigDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
	Result.SavedDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());
	Result.PluginsDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());
	Result.IntermediateDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectIntermediateDir());
	Result.LogDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
	Result.ProjectFilePath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());
	Result.EngineDir = FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	return Result;
}

FString FProjectSettingsImplModule::ResolveConfigFilename(const FString& ConfigName) const
{
	if (ConfigName.Equals(TEXT("Engine"), ESearchCase::IgnoreCase))
	{
		return GEngineIni;
	}
	if (ConfigName.Equals(TEXT("Game"), ESearchCase::IgnoreCase))
	{
		return GGameIni;
	}
	if (ConfigName.Equals(TEXT("Editor"), ESearchCase::IgnoreCase))
	{
		return GEditorIni;
	}
	if (ConfigName.Equals(TEXT("Input"), ESearchCase::IgnoreCase))
	{
		return GInputIni;
	}
	if (ConfigName.Equals(TEXT("Scalability"), ESearchCase::IgnoreCase))
	{
		return GScalabilityIni;
	}
	return FString();
}
