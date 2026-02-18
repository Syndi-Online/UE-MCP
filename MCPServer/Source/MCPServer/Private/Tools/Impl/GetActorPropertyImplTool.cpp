// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetActorPropertyImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FGetActorPropertyImplTool::FGetActorPropertyImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FGetActorPropertyImplTool::GetName() const
{
	return TEXT("get_actor_property");
}

FString FGetActorPropertyImplTool::GetDescription() const
{
	return TEXT("Get the value of a property on an actor by property name.");
}

TSharedPtr<FJsonObject> FGetActorPropertyImplTool::GetInputSchema() const
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
		TEXT("Name of the property to read (e.g. 'bHidden', 'Mobility', 'Tags')"));
	Properties->SetObjectField(TEXT("property_name"), PropertyNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetActorPropertyImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier, PropertyName;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier) ||
		!Arguments->TryGetStringField(TEXT("property_name"), PropertyName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: actor_identifier and property_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FActorPropertyResult PropResult = ActorModule.GetActorProperty(ActorIdentifier, PropertyName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PropResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Property '%s' (%s): %s"),
			*PropResult.PropertyName, *PropResult.PropertyType, *PropResult.PropertyValue);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get property: %s"), *PropResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
