// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/MoveActorsToLevelImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FMoveActorsToLevelImplTool::FMoveActorsToLevelImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FMoveActorsToLevelImplTool::GetName() const
{
	return TEXT("move_actors_to_level");
}

FString FMoveActorsToLevelImplTool::GetDescription() const
{
	return TEXT("Move one or more actors to a different streaming level.");
}

TSharedPtr<FJsonObject> FMoveActorsToLevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifiersProp = MakeShared<FJsonObject>();
	IdentifiersProp->SetStringField(TEXT("type"), TEXT("array"));
	IdentifiersProp->SetStringField(TEXT("description"),
		TEXT("Array of actor names, labels, or paths to move"));
	TSharedPtr<FJsonObject> ItemsProp = MakeShared<FJsonObject>();
	ItemsProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifiersProp->SetObjectField(TEXT("items"), ItemsProp);
	Properties->SetObjectField(TEXT("actor_identifiers"), IdentifiersProp);

	TSharedPtr<FJsonObject> LevelProp = MakeShared<FJsonObject>();
	LevelProp->SetStringField(TEXT("type"), TEXT("string"));
	LevelProp->SetStringField(TEXT("description"),
		TEXT("Name of the target level (e.g. 'PersistentLevel' or sublevel name)"));
	Properties->SetObjectField(TEXT("level_name"), LevelProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifiers")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("level_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FMoveActorsToLevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LevelName;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("level_name"), LevelName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: actor_identifiers and level_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	const TArray<TSharedPtr<FJsonValue>>* IdentifiersArray;
	if (!Arguments->TryGetArrayField(TEXT("actor_identifiers"), IdentifiersArray))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_identifiers"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	TArray<FString> Identifiers;
	for (const TSharedPtr<FJsonValue>& Value : *IdentifiersArray)
	{
		FString Str;
		if (Value->TryGetString(Str))
		{
			Identifiers.Add(Str);
		}
	}

	FMoveActorsToLevelResult MoveResult = ActorModule.MoveActorsToLevel(Identifiers, LevelName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (MoveResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Moved %d actor(s) to level '%s'"), MoveResult.MovedCount, *LevelName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to move actors: %s"), *MoveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
