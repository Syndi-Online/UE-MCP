// Copyright Epic Games, Inc. All Rights Reserved.
#include "Tools/Impl/GetProjectInfoImplTool.h"
#include "Modules/Interfaces/IProjectSettingsModule.h"
#include "Dom/JsonValue.h"

FGetProjectInfoImplTool::FGetProjectInfoImplTool(IProjectSettingsModule& InModule)
	: ProjectSettingsModule(InModule)
{
}

FString FGetProjectInfoImplTool::GetName() const
{
	return TEXT("get_project_info");
}

FString FGetProjectInfoImplTool::GetDescription() const
{
	return TEXT("Get information about the current project");
}

TSharedPtr<FJsonObject> FGetProjectInfoImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	Schema->SetObjectField(TEXT("properties"), MakeShared<FJsonObject>());
	return Schema;
}

TSharedPtr<FJsonObject> FGetProjectInfoImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FProjectInfoResult InfoResult = ProjectSettingsModule.GetProjectInfo();

	FString Message;
	if (InfoResult.bSuccess)
	{
		Message = FString::Printf(TEXT(
			"Project Name: %s\n"
			"Project Version: %s\n"
			"Description: %s\n"
			"Company Name: %s\n"
			"Engine Version: %s\n"
			"Project File Path: %s"),
			*InfoResult.ProjectName,
			*InfoResult.ProjectVersion,
			*InfoResult.Description,
			*InfoResult.CompanyName,
			*InfoResult.EngineVersion,
			*InfoResult.ProjectFilePath);
	}
	else
	{
		Message = InfoResult.ErrorMessage;
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !InfoResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
