// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/PlayInEditorImplTool.h"
#include "Modules/Interfaces/IPIEModule.h"
#include "Dom/JsonValue.h"

FPlayInEditorImplTool::FPlayInEditorImplTool(IPIEModule& InPIEModule)
	: PIEModule(InPIEModule)
{
}

FString FPlayInEditorImplTool::GetName() const
{
	return TEXT("play_in_editor");
}

FString FPlayInEditorImplTool::GetDescription() const
{
	return TEXT("Start a Play In Editor session");
}

TSharedPtr<FJsonObject> FPlayInEditorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FPlayInEditorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FPIEResult PIEResult = PIEModule.PlayInEditor();

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
