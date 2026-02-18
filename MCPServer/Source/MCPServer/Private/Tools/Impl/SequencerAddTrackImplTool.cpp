// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerAddTrackImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerAddTrackImplTool::FSequencerAddTrackImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerAddTrackImplTool::GetName() const
{
	return TEXT("sequencer_add_track");
}

FString FSequencerAddTrackImplTool::GetDescription() const
{
	return TEXT("Add a track to the sequencer (Transform, Visibility, or class path). Optionally bind to an actor GUID.");
}

TSharedPtr<FJsonObject> FSequencerAddTrackImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> TrackTypeProp = MakeShared<FJsonObject>();
	TrackTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	TrackTypeProp->SetStringField(TEXT("description"), TEXT("Track type: Transform, Visibility, or full class path"));
	Properties->SetObjectField(TEXT("track_type"), TrackTypeProp);

	TSharedPtr<FJsonObject> BindingProp = MakeShared<FJsonObject>();
	BindingProp->SetStringField(TEXT("type"), TEXT("string"));
	BindingProp->SetStringField(TEXT("description"), TEXT("Optional binding GUID from sequencer_add_actor (omit for unbound track)"));
	Properties->SetObjectField(TEXT("binding_guid"), BindingProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("track_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSequencerAddTrackImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString TrackType;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("track_type"), TrackType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: track_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString BindingGuid;
	Arguments->TryGetStringField(TEXT("binding_guid"), BindingGuid);

	FSequencerAddTrackResult TrackResult = SequencerModule.SequencerAddTrack(TrackType, BindingGuid);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (TrackResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Track '%s' added to sequencer."), *TrackResult.TrackName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add track: %s"), *TrackResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
