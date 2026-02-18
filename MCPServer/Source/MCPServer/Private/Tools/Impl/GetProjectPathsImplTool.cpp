// Copyright Epic Games, Inc. All Rights Reserved.
#include "Tools/Impl/GetProjectPathsImplTool.h"
#include "Modules/Interfaces/IProjectSettingsModule.h"
#include "Dom/JsonValue.h"

FGetProjectPathsImplTool::FGetProjectPathsImplTool(IProjectSettingsModule& InModule)
	: ProjectSettingsModule(InModule)
{
}

FString FGetProjectPathsImplTool::GetName() const
{
	return TEXT("get_project_paths");
}

FString FGetProjectPathsImplTool::GetDescription() const
{
	return TEXT("Get important project directory paths");
}

TSharedPtr<FJsonObject> FGetProjectPathsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	Schema->SetObjectField(TEXT("properties"), MakeShared<FJsonObject>());
	return Schema;
}

TSharedPtr<FJsonObject> FGetProjectPathsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FProjectPathsResult PathsResult = ProjectSettingsModule.GetProjectPaths();

	FString Message;
	if (PathsResult.bSuccess)
	{
		Message = FString::Printf(TEXT(
			"Project: %s\n"
			"Content: %s\n"
			"Config: %s\n"
			"Saved: %s\n"
			"Plugins: %s\n"
			"Intermediate: %s\n"
			"Logs: %s\n"
			"Project File: %s\n"
			"Engine: %s"),
			*PathsResult.ProjectDir,
			*PathsResult.ContentDir,
			*PathsResult.ConfigDir,
			*PathsResult.SavedDir,
			*PathsResult.PluginsDir,
			*PathsResult.IntermediateDir,
			*PathsResult.LogDir,
			*PathsResult.ProjectFilePath,
			*PathsResult.EngineDir);
	}
	else
	{
		Message = TEXT("Failed to retrieve project paths.");
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !PathsResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
