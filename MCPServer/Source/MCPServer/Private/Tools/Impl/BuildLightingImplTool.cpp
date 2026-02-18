// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BuildLightingImplTool.h"
#include "Modules/Interfaces/IBuildModule.h"
#include "Dom/JsonValue.h"

FBuildLightingImplTool::FBuildLightingImplTool(IBuildModule& InBuildModule)
	: BuildModule(InBuildModule)
{
}

FString FBuildLightingImplTool::GetName() const
{
	return TEXT("build_lighting");
}

FString FBuildLightingImplTool::GetDescription() const
{
	return TEXT("Build lighting (lightmaps) for the current level");
}

TSharedPtr<FJsonObject> FBuildLightingImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FBuildLightingImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FEditorBuildResult BuildResult = BuildModule.BuildLighting();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Lighting build started successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to build lighting: %s"), *BuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
