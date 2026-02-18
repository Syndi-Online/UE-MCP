// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerPlayImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerPlayImplTool::FSequencerPlayImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerPlayImplTool::GetName() const
{
	return TEXT("sequencer_play");
}

FString FSequencerPlayImplTool::GetDescription() const
{
	return TEXT("Start playback of the active Level Sequence in the editor");
}

TSharedPtr<FJsonObject> FSequencerPlayImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);
	return Schema;
}

TSharedPtr<FJsonObject> FSequencerPlayImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSequencerSimpleResult PlayResult = SequencerModule.SequencerPlay();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PlayResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Sequencer playback started."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to start playback: %s"), *PlayResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
