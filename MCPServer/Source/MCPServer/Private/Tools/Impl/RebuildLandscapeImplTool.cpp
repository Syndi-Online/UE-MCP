// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RebuildLandscapeImplTool.h"
#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "Dom/JsonValue.h"

FRebuildLandscapeImplTool::FRebuildLandscapeImplTool(ILandscapeToolModule& InLandscapeModule)
	: LandscapeModule(InLandscapeModule)
{
}

FString FRebuildLandscapeImplTool::GetName() const
{
	return TEXT("rebuild_landscape");
}

FString FRebuildLandscapeImplTool::GetDescription() const
{
	return TEXT("Rebuild landscape data: collision, grass maps, and force layer updates");
}

TSharedPtr<FJsonObject> FRebuildLandscapeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"), TEXT("Name of the landscape actor (empty for first landscape in level)"));
	Properties->SetObjectField(TEXT("landscape_name"), NameProp);
	Schema->SetObjectField(TEXT("properties"), Properties);
	return Schema;
}

TSharedPtr<FJsonObject> FRebuildLandscapeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LandscapeName;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("landscape_name"), LandscapeName);
	}

	FLandscapeRebuildResult RebuildResult = LandscapeModule.RebuildLandscape(LandscapeName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (RebuildResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Landscape data rebuilt successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to rebuild landscape: %s"), *RebuildResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
