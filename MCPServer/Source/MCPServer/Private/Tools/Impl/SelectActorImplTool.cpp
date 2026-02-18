// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SelectActorImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FSelectActorImplTool::FSelectActorImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FSelectActorImplTool::GetName() const
{
	return TEXT("select_actor");
}

FString FSelectActorImplTool::GetDescription() const
{
	return TEXT("Select one or more actors in the editor. Can replace or add to the current selection.");
}

TSharedPtr<FJsonObject> FSelectActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifiersProp = MakeShared<FJsonObject>();
	IdentifiersProp->SetStringField(TEXT("type"), TEXT("array"));
	IdentifiersProp->SetStringField(TEXT("description"),
		TEXT("Array of actor names, labels, or paths to select"));
	TSharedPtr<FJsonObject> ItemsProp = MakeShared<FJsonObject>();
	ItemsProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifiersProp->SetObjectField(TEXT("items"), ItemsProp);
	Properties->SetObjectField(TEXT("actor_identifiers"), IdentifiersProp);

	TSharedPtr<FJsonObject> AdditiveProp = MakeShared<FJsonObject>();
	AdditiveProp->SetStringField(TEXT("type"), TEXT("boolean"));
	AdditiveProp->SetStringField(TEXT("description"),
		TEXT("If true, add to existing selection instead of replacing it. Default: false."));
	Properties->SetObjectField(TEXT("add_to_selection"), AdditiveProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifiers")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSelectActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	if (!Arguments.IsValid())
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_identifiers"));
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

	bool bAddToSelection = false;
	Arguments->TryGetBoolField(TEXT("add_to_selection"), bAddToSelection);

	FActorSelectionResult SelectResult = ActorModule.SelectActors(Identifiers, bAddToSelection);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SelectResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Selected %d actor(s)"), SelectResult.AffectedCount));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to select actors: %s"), *SelectResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
