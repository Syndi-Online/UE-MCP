// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerSetPlaybackRangeImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerSetPlaybackRangeImplTool::FSequencerSetPlaybackRangeImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerSetPlaybackRangeImplTool::GetName() const
{
	return TEXT("sequencer_set_playback_range");
}

FString FSequencerSetPlaybackRangeImplTool::GetDescription() const
{
	return TEXT("Set the playback range (start and end time in seconds) for the sequence");
}

TSharedPtr<FJsonObject> FSequencerSetPlaybackRangeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> StartProp = MakeShared<FJsonObject>();
	StartProp->SetStringField(TEXT("type"), TEXT("number"));
	StartProp->SetStringField(TEXT("description"), TEXT("Start time in seconds"));
	Properties->SetObjectField(TEXT("start_time_seconds"), StartProp);

	TSharedPtr<FJsonObject> EndProp = MakeShared<FJsonObject>();
	EndProp->SetStringField(TEXT("type"), TEXT("number"));
	EndProp->SetStringField(TEXT("description"), TEXT("End time in seconds"));
	Properties->SetObjectField(TEXT("end_time_seconds"), EndProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("start_time_seconds")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("end_time_seconds")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSequencerSetPlaybackRangeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	if (!Arguments.IsValid() || !Arguments->HasField(TEXT("start_time_seconds")) || !Arguments->HasField(TEXT("end_time_seconds")))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: start_time_seconds, end_time_seconds"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	double StartTime = Arguments->GetNumberField(TEXT("start_time_seconds"));
	double EndTime = Arguments->GetNumberField(TEXT("end_time_seconds"));

	FSequencerSimpleResult SetResult = SequencerModule.SequencerSetPlaybackRange(StartTime, EndTime);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Playback range set to %.3f - %.3f seconds."), StartTime, EndTime));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set playback range: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
