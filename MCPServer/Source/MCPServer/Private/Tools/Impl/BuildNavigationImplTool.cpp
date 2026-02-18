// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BuildNavigationImplTool.h"
#include "Modules/Interfaces/INavigationModule.h"
#include "Dom/JsonValue.h"

FBuildNavigationImplTool::FBuildNavigationImplTool(INavigationModule& InNavigationModule)
	: NavigationModule(InNavigationModule)
{
}

FString FBuildNavigationImplTool::GetName() const
{
	return TEXT("build_navigation");
}

FString FBuildNavigationImplTool::GetDescription() const
{
	return TEXT("Build navigation mesh for the current level");
}

TSharedPtr<FJsonObject> FBuildNavigationImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FBuildNavigationImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FNavBuildResult BuildResult = NavigationModule.BuildNavigation();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Navigation mesh built successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to build navigation mesh: %s"), *BuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
