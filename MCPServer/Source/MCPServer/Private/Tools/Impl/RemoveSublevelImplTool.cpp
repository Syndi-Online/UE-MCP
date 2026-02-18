// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RemoveSublevelImplTool.h"
#include "Modules/Interfaces/ILevelModule.h"
#include "Dom/JsonValue.h"

FRemoveSublevelImplTool::FRemoveSublevelImplTool(ILevelModule& InLevelModule)
	: LevelModule(InLevelModule)
{
}

FString FRemoveSublevelImplTool::GetName() const
{
	return TEXT("remove_sublevel");
}

FString FRemoveSublevelImplTool::GetDescription() const
{
	return TEXT("Remove a streaming sublevel from the current world.");
}

TSharedPtr<FJsonObject> FRemoveSublevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"),
		TEXT("Name of the sublevel to remove (partial match supported)"));
	Properties->SetObjectField(TEXT("level_name"), NameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FRemoveSublevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FRemoveSublevelResult RemoveResult = LevelModule.RemoveSublevel(LevelName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (RemoveResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Sublevel removed: %s"), *RemoveResult.SublevelName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to remove sublevel: %s"), *RemoveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
