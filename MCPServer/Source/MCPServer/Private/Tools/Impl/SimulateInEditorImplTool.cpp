// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SimulateInEditorImplTool.h"
#include "Modules/Interfaces/IPIEModule.h"
#include "Dom/JsonValue.h"

FSimulateInEditorImplTool::FSimulateInEditorImplTool(IPIEModule& InPIEModule)
	: PIEModule(InPIEModule)
{
}

FString FSimulateInEditorImplTool::GetName() const
{
	return TEXT("simulate_in_editor");
}

FString FSimulateInEditorImplTool::GetDescription() const
{
	return TEXT("Start a Simulate In Editor session");
}

TSharedPtr<FJsonObject> FSimulateInEditorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FSimulateInEditorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FPIEResult PIEResult = PIEModule.SimulateInEditor();

	FString Message = PIEResult.bSuccess ? PIEResult.Message : PIEResult.ErrorMessage;

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);

	Result->SetBoolField(TEXT("isError"), !PIEResult.bSuccess);

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
