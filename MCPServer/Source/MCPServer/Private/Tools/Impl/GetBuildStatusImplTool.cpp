// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetBuildStatusImplTool.h"
#include "Modules/Interfaces/IBuildModule.h"
#include "Dom/JsonValue.h"

FGetBuildStatusImplTool::FGetBuildStatusImplTool(IBuildModule& InBuildModule)
	: BuildModule(InBuildModule)
{
}

FString FGetBuildStatusImplTool::GetName() const
{
	return TEXT("get_build_status");
}

FString FGetBuildStatusImplTool::GetDescription() const
{
	return TEXT("Get the current build status");
}

TSharedPtr<FJsonObject> FGetBuildStatusImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FGetBuildStatusImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FBuildStatusResult StatusResult = BuildModule.GetBuildStatus();

	FString StatusMessage = FString::Printf(
		TEXT("Build Status:\n- Building: %s\n- Lighting Build: %s\n- Navigation Build: %s\n- Cancelled: %s"),
		StatusResult.bIsBuilding ? TEXT("yes") : TEXT("no"),
		StatusResult.bIsLightingBuilding ? TEXT("yes") : TEXT("no"),
		StatusResult.bIsNavigationBuilding ? TEXT("yes") : TEXT("no"),
		StatusResult.bWasCancelled ? TEXT("yes") : TEXT("no"));

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), StatusMessage);

	Result->SetBoolField(TEXT("isError"), false);

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
