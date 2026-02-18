// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/StopPieImplTool.h"
#include "Modules/Interfaces/IPIEModule.h"
#include "Dom/JsonValue.h"

FStopPieImplTool::FStopPieImplTool(IPIEModule& InPIEModule)
	: PIEModule(InPIEModule)
{
}

FString FStopPieImplTool::GetName() const
{
	return TEXT("stop_pie");
}

FString FStopPieImplTool::GetDescription() const
{
	return TEXT("Stop the current Play In Editor session");
}

TSharedPtr<FJsonObject> FStopPieImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FStopPieImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FPIEResult PIEResult = PIEModule.StopPIE();

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
