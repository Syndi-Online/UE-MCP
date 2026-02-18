// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/FocusOnActorsImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FFocusOnActorsImplTool::FFocusOnActorsImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FFocusOnActorsImplTool::GetName() const
{
	return TEXT("focus_on_actors");
}

FString FFocusOnActorsImplTool::GetDescription() const
{
	return TEXT("Focus the viewport camera on one or more actors.");
}

TSharedPtr<FJsonObject> FFocusOnActorsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifiersProp = MakeShared<FJsonObject>();
	IdentifiersProp->SetStringField(TEXT("type"), TEXT("array"));
	TSharedPtr<FJsonObject> ItemsProp = MakeShared<FJsonObject>();
	ItemsProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifiersProp->SetObjectField(TEXT("items"), ItemsProp);
	Properties->SetObjectField(TEXT("actor_identifiers"), IdentifiersProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifiers")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FFocusOnActorsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FSimpleResult FocusResult = ViewportModule.FocusOnActors(Identifiers);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (FocusResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), FocusResult.Message);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to focus on actors: %s"), *FocusResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
