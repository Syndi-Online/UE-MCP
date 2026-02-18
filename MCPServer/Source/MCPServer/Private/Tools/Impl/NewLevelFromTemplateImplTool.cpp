// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/NewLevelFromTemplateImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FNewLevelFromTemplateImplTool::FNewLevelFromTemplateImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FNewLevelFromTemplateImplTool::GetName() const
{
	return TEXT("new_level_from_template");
}

FString FNewLevelFromTemplateImplTool::GetDescription() const
{
	return TEXT("Create a new level from an existing template level.");
}

TSharedPtr<FJsonObject> FNewLevelFromTemplateImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PathProp = MakeShared<FJsonObject>();
	PathProp->SetStringField(TEXT("type"), TEXT("string"));
	PathProp->SetStringField(TEXT("description"),
		TEXT("Asset path for the new level (e.g. '/Game/Maps/NewMap')"));
	Properties->SetObjectField(TEXT("level_path"), PathProp);

	TSharedPtr<FJsonObject> TemplateProp = MakeShared<FJsonObject>();
	TemplateProp->SetStringField(TEXT("type"), TEXT("string"));
	TemplateProp->SetStringField(TEXT("description"),
		TEXT("Asset path of the template level (e.g. '/Game/Maps/Template')"));
	Properties->SetObjectField(TEXT("template_path"), TemplateProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("template_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FNewLevelFromTemplateImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LevelPath, TemplatePath;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("level_path"), LevelPath) ||
		!Arguments->TryGetStringField(TEXT("template_path"), TemplatePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: level_path and template_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FNewLevelResult LevelResult = LevelModule.NewLevelFromTemplate(LevelPath, TemplatePath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LevelResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Level created from template: %s (%s)"), *LevelResult.LevelName, *LevelResult.LevelPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create level from template: %s"), *LevelResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
