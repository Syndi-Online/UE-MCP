// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/PilotActorImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FPilotActorImplTool::FPilotActorImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FPilotActorImplTool::GetName() const
{
	return TEXT("pilot_actor");
}

FString FPilotActorImplTool::GetDescription() const
{
	return TEXT("Pilot (lock camera to) an actor in the viewport.");
}

TSharedPtr<FJsonObject> FPilotActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FPilotActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_identifier"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSimpleResult PilotResult = ViewportModule.PilotActor(ActorIdentifier);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PilotResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), PilotResult.Message);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to pilot actor: %s"), *PilotResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
