// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ConvertActorImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FConvertActorImplTool::FConvertActorImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FConvertActorImplTool::GetName() const
{
	return TEXT("convert_actor");
}

FString FConvertActorImplTool::GetDescription() const
{
	return TEXT("Convert an actor to a different class type. Preserves transform but destroys old actor.");
}

TSharedPtr<FJsonObject> FConvertActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"), TEXT("Actor name, label, or path to convert"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	TSharedPtr<FJsonObject> ClassProp = MakeShared<FJsonObject>();
	ClassProp->SetStringField(TEXT("type"), TEXT("string"));
	ClassProp->SetStringField(TEXT("description"),
		TEXT("Full class path of the target type (e.g. '/Script/Engine.PointLight')"));
	Properties->SetObjectField(TEXT("new_class"), ClassProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("new_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FConvertActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier, NewClass;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier) ||
		!Arguments->TryGetStringField(TEXT("new_class"), NewClass))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: actor_identifier and new_class"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FActorConvertResult ConvertResult = ActorModule.ConvertActor(ActorIdentifier, NewClass);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ConvertResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Actor converted successfully.\nNew Name: %s\nNew Label: %s\nNew Class: %s"),
			*ConvertResult.NewActorName, *ConvertResult.NewActorLabel, *ConvertResult.NewActorClass);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to convert actor: %s"), *ConvertResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
