// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SaveLevelImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FSaveLevelImplTool::FSaveLevelImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FSaveLevelImplTool::GetName() const
{
	return TEXT("save_level");
}

FString FSaveLevelImplTool::GetDescription() const
{
	return TEXT("Save the current level.");
}

TSharedPtr<FJsonObject> FSaveLevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FSaveLevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSaveLevelResult SaveResult = LevelModule.SaveLevel();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SaveResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Level saved: %s"), *SaveResult.LevelName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to save level: %s"), *SaveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
