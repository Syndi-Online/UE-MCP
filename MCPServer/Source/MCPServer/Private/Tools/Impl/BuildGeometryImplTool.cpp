// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BuildGeometryImplTool.h"
#include "Modules/Interfaces/IBuildModule.h"
#include "Dom/JsonValue.h"

FBuildGeometryImplTool::FBuildGeometryImplTool(IBuildModule& InBuildModule)
	: BuildModule(InBuildModule)
{
}

FString FBuildGeometryImplTool::GetName() const
{
	return TEXT("build_geometry");
}

FString FBuildGeometryImplTool::GetDescription() const
{
	return TEXT("Build BSP geometry for the current level");
}

TSharedPtr<FJsonObject> FBuildGeometryImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FBuildGeometryImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FEditorBuildResult BuildResult = BuildModule.BuildGeometry();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Geometry built successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to build geometry: %s"), *BuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
