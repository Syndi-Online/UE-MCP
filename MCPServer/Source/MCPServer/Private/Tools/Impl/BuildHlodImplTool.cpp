// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BuildHlodImplTool.h"
#include "Modules/Interfaces/IBuildModule.h"
#include "Dom/JsonValue.h"

FBuildHlodImplTool::FBuildHlodImplTool(IBuildModule& InBuildModule)
	: BuildModule(InBuildModule)
{
}

FString FBuildHlodImplTool::GetName() const
{
	return TEXT("build_hlod");
}

FString FBuildHlodImplTool::GetDescription() const
{
	return TEXT("Build Hierarchical LOD clusters for the current level");
}

TSharedPtr<FJsonObject> FBuildHlodImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FBuildHlodImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FEditorBuildResult BuildResult = BuildModule.BuildHLOD();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("HLOD built successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to build HLOD: %s"), *BuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
