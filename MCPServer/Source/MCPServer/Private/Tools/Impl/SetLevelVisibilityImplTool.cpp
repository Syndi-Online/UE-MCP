// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetLevelVisibilityImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FSetLevelVisibilityImplTool::FSetLevelVisibilityImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FSetLevelVisibilityImplTool::GetName() const
{
	return TEXT("set_level_visibility");
}

FString FSetLevelVisibilityImplTool::GetDescription() const
{
	return TEXT("Set the visibility of a streaming level in the editor.");
}

TSharedPtr<FJsonObject> FSetLevelVisibilityImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"),
		TEXT("Name of the level (e.g. 'PersistentLevel' or sublevel name, partial match supported)"));
	Properties->SetObjectField(TEXT("level_name"), NameProp);

	TSharedPtr<FJsonObject> VisibleProp = MakeShared<FJsonObject>();
	VisibleProp->SetStringField(TEXT("type"), TEXT("boolean"));
	VisibleProp->SetStringField(TEXT("description"),
		TEXT("Whether the level should be visible (true) or hidden (false)"));
	Properties->SetObjectField(TEXT("visible"), VisibleProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("visible")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetLevelVisibilityImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LevelName;
	bool bVisible = true;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("level_name"), LevelName) ||
		!Arguments->TryGetBoolField(TEXT("visible"), bVisible))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: level_name and visible"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetLevelVisibilityResult VisResult = LevelModule.SetLevelVisibility(LevelName, bVisible);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (VisResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Level '%s' visibility set to %s"),
				*VisResult.LevelName, VisResult.bVisible ? TEXT("visible") : TEXT("hidden")));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set level visibility: %s"), *VisResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
