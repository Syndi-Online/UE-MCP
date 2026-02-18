// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RebuildNavigationImplTool.h"
#include "Modules/Interfaces/INavigationModule.h"
#include "Dom/JsonValue.h"

FRebuildNavigationImplTool::FRebuildNavigationImplTool(INavigationModule& InNavigationModule)
	: NavigationModule(InNavigationModule)
{
}

FString FRebuildNavigationImplTool::GetName() const
{
	return TEXT("rebuild_navigation");
}

FString FRebuildNavigationImplTool::GetDescription() const
{
	return TEXT("Rebuild navigation mesh from scratch");
}

TSharedPtr<FJsonObject> FRebuildNavigationImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FRebuildNavigationImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FNavBuildResult RebuildResult = NavigationModule.RebuildNavigation();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (RebuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Navigation mesh rebuilt successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to rebuild navigation mesh: %s"), *RebuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
