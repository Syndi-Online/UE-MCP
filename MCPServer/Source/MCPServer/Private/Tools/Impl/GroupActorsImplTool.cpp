// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GroupActorsImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FGroupActorsImplTool::FGroupActorsImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FGroupActorsImplTool::GetName() const
{
	return TEXT("group_actors");
}

FString FGroupActorsImplTool::GetDescription() const
{
	return TEXT("Group multiple actors together in the editor.");
}

TSharedPtr<FJsonObject> FGroupActorsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifiersProp = MakeShared<FJsonObject>();
	IdentifiersProp->SetStringField(TEXT("type"), TEXT("array"));
	IdentifiersProp->SetStringField(TEXT("description"),
		TEXT("Array of actor names, labels, or paths to group (minimum 2)"));
	TSharedPtr<FJsonObject> ItemsProp = MakeShared<FJsonObject>();
	ItemsProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifiersProp->SetObjectField(TEXT("items"), ItemsProp);
	Properties->SetObjectField(TEXT("actor_identifiers"), IdentifiersProp);

	TSharedPtr<FJsonObject> GroupNameProp = MakeShared<FJsonObject>();
	GroupNameProp->SetStringField(TEXT("type"), TEXT("string"));
	GroupNameProp->SetStringField(TEXT("description"),
		TEXT("Optional name for the group"));
	Properties->SetObjectField(TEXT("group_name"), GroupNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifiers")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGroupActorsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString GroupName;
	Arguments->TryGetStringField(TEXT("group_name"), GroupName);

	FGroupActorsResult GroupResult = ActorModule.GroupActors(Identifiers, GroupName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (GroupResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Grouped %d actors into '%s'"), GroupResult.ActorCount, *GroupResult.GroupName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to group actors: %s"), *GroupResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
