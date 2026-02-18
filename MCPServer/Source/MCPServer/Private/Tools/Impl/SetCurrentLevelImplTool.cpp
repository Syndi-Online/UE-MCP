// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetCurrentLevelImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FSetCurrentLevelImplTool::FSetCurrentLevelImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FSetCurrentLevelImplTool::GetName() const
{
	return TEXT("set_current_level");
}

FString FSetCurrentLevelImplTool::GetDescription() const
{
	return TEXT("Set the current working level for new actor placement.");
}

TSharedPtr<FJsonObject> FSetCurrentLevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"),
		TEXT("Name of the level to make current (e.g. 'PersistentLevel' or sublevel name)"));
	Properties->SetObjectField(TEXT("level_name"), NameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetCurrentLevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LevelName;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("level_name"), LevelName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: level_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetCurrentLevelResult SetResult = LevelModule.SetCurrentLevel(LevelName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Current level set to: %s"), *SetResult.LevelName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set current level: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
