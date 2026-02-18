// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/NewLevelImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FNewLevelImplTool::FNewLevelImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FNewLevelImplTool::GetName() const
{
	return TEXT("new_level");
}

FString FNewLevelImplTool::GetDescription() const
{
	return TEXT("Create a new empty level at the specified asset path.");
}

TSharedPtr<FJsonObject> FNewLevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PathProp = MakeShared<FJsonObject>();
	PathProp->SetStringField(TEXT("type"), TEXT("string"));
	PathProp->SetStringField(TEXT("description"),
		TEXT("Asset path for the new level (e.g. '/Game/Maps/NewMap')"));
	Properties->SetObjectField(TEXT("level_path"), PathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FNewLevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LevelPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("level_path"), LevelPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: level_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FNewLevelResult LevelResult = LevelModule.NewLevel(LevelPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LevelResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("New level created: %s (%s)"), *LevelResult.LevelName, *LevelResult.LevelPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create level: %s"), *LevelResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
