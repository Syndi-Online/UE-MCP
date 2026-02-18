// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerAddActorImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerAddActorImplTool::FSequencerAddActorImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerAddActorImplTool::GetName() const
{
	return TEXT("sequencer_add_actor");
}

FString FSequencerAddActorImplTool::GetDescription() const
{
	return TEXT("Add an actor from the level to the active sequencer as a possessable");
}

TSharedPtr<FJsonObject> FSequencerAddActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorNameProp = MakeShared<FJsonObject>();
	ActorNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ActorNameProp->SetStringField(TEXT("description"), TEXT("Name or label of the actor to add"));
	Properties->SetObjectField(TEXT("actor_name"), ActorNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSequencerAddActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorName;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_name"), ActorName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSequencerAddActorResult AddResult = SequencerModule.SequencerAddActor(ActorName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (AddResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Actor '%s' added to sequencer. Binding GUID: %s"), *ActorName, *AddResult.BindingGuid));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add actor: %s"), *AddResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
