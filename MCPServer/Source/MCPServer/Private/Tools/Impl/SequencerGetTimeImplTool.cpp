// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerGetTimeImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerGetTimeImplTool::FSequencerGetTimeImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerGetTimeImplTool::GetName() const
{
	return TEXT("sequencer_get_time");
}

FString FSequencerGetTimeImplTool::GetDescription() const
{
	return TEXT("Get the current playback position from the sequencer timeline");
}

TSharedPtr<FJsonObject> FSequencerGetTimeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);
	return Schema;
}

TSharedPtr<FJsonObject> FSequencerGetTimeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSequencerTimeResult TimeResult = SequencerModule.SequencerGetTime();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (TimeResult.bSuccess)
	{
		TSharedPtr<FJsonObject> InfoJson = MakeShared<FJsonObject>();
		InfoJson->SetNumberField(TEXT("time_seconds"), TimeResult.TimeInSeconds);
		InfoJson->SetNumberField(TEXT("frame_number"), TimeResult.FrameNumber);
		InfoJson->SetNumberField(TEXT("frame_rate"), TimeResult.FrameRate);

		FString ResponseText;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseText);
		FJsonSerializer::Serialize(InfoJson.ToSharedRef(), Writer);

		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get time: %s"), *TimeResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
