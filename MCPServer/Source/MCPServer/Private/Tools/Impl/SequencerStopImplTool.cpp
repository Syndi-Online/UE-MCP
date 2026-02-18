// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerStopImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerStopImplTool::FSequencerStopImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerStopImplTool::GetName() const
{
	return TEXT("sequencer_stop");
}

FString FSequencerStopImplTool::GetDescription() const
{
	return TEXT("Stop playback and return to the beginning of the sequence");
}

TSharedPtr<FJsonObject> FSequencerStopImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);
	return Schema;
}

TSharedPtr<FJsonObject> FSequencerStopImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSequencerSimpleResult StopResult = SequencerModule.SequencerStop();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (StopResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Sequencer stopped and returned to start."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to stop: %s"), *StopResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
