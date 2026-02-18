// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/LoadLevelImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FLoadLevelImplTool::FLoadLevelImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FLoadLevelImplTool::GetName() const
{
	return TEXT("load_level");
}

FString FLoadLevelImplTool::GetDescription() const
{
	return TEXT("Load/open an existing level by asset path.");
}

TSharedPtr<FJsonObject> FLoadLevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PathProp = MakeShared<FJsonObject>();
	PathProp->SetStringField(TEXT("type"), TEXT("string"));
	PathProp->SetStringField(TEXT("description"),
		TEXT("Asset path of the level to load (e.g. '/Game/Maps/MyLevel')"));
	Properties->SetObjectField(TEXT("level_path"), PathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FLoadLevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FLoadLevelResult LoadResult = LevelModule.LoadLevel(LevelPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LoadResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Level loaded: %s"), *LoadResult.LevelName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to load level: %s"), *LoadResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
