// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerAddMarkerImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerAddMarkerImplTool::FSequencerAddMarkerImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerAddMarkerImplTool::GetName() const
{
	return TEXT("sequencer_add_marker");
}

FString FSequencerAddMarkerImplTool::GetDescription() const
{
	return TEXT("Add a labeled marker at a specific time on the sequencer timeline");
}

TSharedPtr<FJsonObject> FSequencerAddMarkerImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> TimeProp = MakeShared<FJsonObject>();
	TimeProp->SetStringField(TEXT("type"), TEXT("number"));
	TimeProp->SetStringField(TEXT("description"), TEXT("Time in seconds where the marker will be placed"));
	Properties->SetObjectField(TEXT("time_seconds"), TimeProp);

	TSharedPtr<FJsonObject> LabelProp = MakeShared<FJsonObject>();
	LabelProp->SetStringField(TEXT("type"), TEXT("string"));
	LabelProp->SetStringField(TEXT("description"), TEXT("Label for the marker"));
	Properties->SetObjectField(TEXT("label"), LabelProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("time_seconds")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("label")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSequencerAddMarkerImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	if (!Arguments.IsValid() || !Arguments->HasField(TEXT("time_seconds")))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: time_seconds"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString Label;
	if (!Arguments->TryGetStringField(TEXT("label"), Label))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: label"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	double TimeSeconds = Arguments->GetNumberField(TEXT("time_seconds"));

	FSequencerAddMarkerResult MarkerResult = SequencerModule.SequencerAddMarker(TimeSeconds, Label);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (MarkerResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Marker '%s' added at %.3f seconds (index: %d)."), *Label, TimeSeconds, MarkerResult.MarkerIndex));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add marker: %s"), *MarkerResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
