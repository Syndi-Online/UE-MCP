// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BuildNavigationDataImplTool.h"
#include "Modules/Interfaces/IBuildModule.h"
#include "Dom/JsonValue.h"

FBuildNavigationDataImplTool::FBuildNavigationDataImplTool(IBuildModule& InBuildModule)
	: BuildModule(InBuildModule)
{
}

FString FBuildNavigationDataImplTool::GetName() const
{
	return TEXT("build_navigation_data");
}

FString FBuildNavigationDataImplTool::GetDescription() const
{
	return TEXT("Build AI navigation paths for the current level");
}

TSharedPtr<FJsonObject> FBuildNavigationDataImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FBuildNavigationDataImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FEditorBuildResult BuildResult = BuildModule.BuildNavigationData();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Navigation data built successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to build navigation data: %s"), *BuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
