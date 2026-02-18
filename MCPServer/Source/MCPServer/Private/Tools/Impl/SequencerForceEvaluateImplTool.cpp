// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SequencerForceEvaluateImplTool.h"
#include "Modules/Interfaces/ISequencerToolModule.h"
#include "Dom/JsonValue.h"

FSequencerForceEvaluateImplTool::FSequencerForceEvaluateImplTool(ISequencerToolModule& InSequencerModule)
	: SequencerModule(InSequencerModule)
{
}

FString FSequencerForceEvaluateImplTool::GetName() const
{
	return TEXT("sequencer_force_evaluate");
}

FString FSequencerForceEvaluateImplTool::GetDescription() const
{
	return TEXT("Force the sequencer to re-evaluate at the current time");
}

TSharedPtr<FJsonObject> FSequencerForceEvaluateImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);
	return Schema;
}

TSharedPtr<FJsonObject> FSequencerForceEvaluateImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSequencerSimpleResult EvalResult = SequencerModule.SequencerForceEvaluate();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (EvalResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Sequencer force-evaluated at current time."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to force evaluate: %s"), *EvalResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
