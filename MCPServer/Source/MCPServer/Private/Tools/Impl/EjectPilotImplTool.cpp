// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/EjectPilotImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FEjectPilotImplTool::FEjectPilotImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FEjectPilotImplTool::GetName() const
{
	return TEXT("eject_pilot");
}

FString FEjectPilotImplTool::GetDescription() const
{
	return TEXT("Eject from the currently piloted actor.");
}

TSharedPtr<FJsonObject> FEjectPilotImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FEjectPilotImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FSimpleResult EjectResult = ViewportModule.EjectPilot();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (EjectResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), EjectResult.Message);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to eject pilot: %s"), *EjectResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
