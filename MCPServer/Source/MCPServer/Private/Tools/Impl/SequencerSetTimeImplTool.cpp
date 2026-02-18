// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerSetTimeImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerSetTimeImplTool::FSequencerSetTimeImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerSetTimeImplTool::GetName() const
{
	return TEXT("sequencer_set_time");
}

FString FSequencerSetTimeImplTool::GetDescription() const
{
	return TEXT("Set the current playback position on the sequencer timeline (in seconds)");
}

TSharedPtr<FJsonObject> FSequencerSetTimeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> TimeProp = MakeShared<FJsonObject>();
	TimeProp->SetStringField(TEXT("type"), TEXT("number"));
	TimeProp->SetStringField(TEXT("description"), TEXT("Time position in seconds"));
	Properties->SetObjectField(TEXT("time_seconds"), TimeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("time_seconds")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSequencerSetTimeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	double TimeSeconds = 0.0;
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
	TimeSeconds = Arguments->GetNumberField(TEXT("time_seconds"));

	FSequencerSimpleResult SetResult = SequencerModule.SequencerSetTime(TimeSeconds);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Sequencer time set to %.3f seconds."), TimeSeconds));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set time: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
