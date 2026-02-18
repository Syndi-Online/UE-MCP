// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetActorPropertyImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FSetActorPropertyImplTool::FSetActorPropertyImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FSetActorPropertyImplTool::GetName() const
{
	return TEXT("set_actor_property");
}

FString FSetActorPropertyImplTool::GetDescription() const
{
	return TEXT("Set the value of a property on an actor. Uses UE text import format for the value.");
}

TSharedPtr<FJsonObject> FSetActorPropertyImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"), TEXT("Actor name, label, or path"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	TSharedPtr<FJsonObject> PropertyNameProp = MakeShared<FJsonObject>();
	PropertyNameProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyNameProp->SetStringField(TEXT("description"),
		TEXT("Name of the property to set (e.g. 'bHidden', 'Mobility')"));
	Properties->SetObjectField(TEXT("property_name"), PropertyNameProp);

	TSharedPtr<FJsonObject> PropertyValueProp = MakeShared<FJsonObject>();
	PropertyValueProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyValueProp->SetStringField(TEXT("description"),
		TEXT("Value to set (as text, e.g. 'true', '1.0', 'Movable')"));
	Properties->SetObjectField(TEXT("property_value"), PropertyValueProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_value")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetActorPropertyImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier, PropertyName, PropertyValue;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier) ||
		!Arguments->TryGetStringField(TEXT("property_name"), PropertyName) ||
		!Arguments->TryGetStringField(TEXT("property_value"), PropertyValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: actor_identifier, property_name, and property_value"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FActorSetPropertyResult SetResult = ActorModule.SetActorProperty(ActorIdentifier, PropertyName, PropertyValue);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Property '%s' set to '%s' successfully"), *PropertyName, *PropertyValue));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set property: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
