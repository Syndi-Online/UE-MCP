// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SaveAllDirtyLevelsImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FSaveAllDirtyLevelsImplTool::FSaveAllDirtyLevelsImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FSaveAllDirtyLevelsImplTool::GetName() const
{
	return TEXT("save_all_dirty_levels");
}

FString FSaveAllDirtyLevelsImplTool::GetDescription() const
{
	return TEXT("Save all modified (dirty) levels.");
}

TSharedPtr<FJsonObject> FSaveAllDirtyLevelsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FSaveAllDirtyLevelsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSaveAllDirtyLevelsResult SaveResult = LevelModule.SaveAllDirtyLevels();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SaveResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("All dirty levels saved successfully"));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to save dirty levels: %s"), *SaveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
