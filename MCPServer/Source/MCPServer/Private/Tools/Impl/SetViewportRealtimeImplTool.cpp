// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetViewportRealtimeImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FSetViewportRealtimeImplTool::FSetViewportRealtimeImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FSetViewportRealtimeImplTool::GetName() const
{
	return TEXT("set_viewport_realtime");
}

FString FSetViewportRealtimeImplTool::GetDescription() const
{
	return TEXT("Toggle realtime rendering in the viewport.");
}

TSharedPtr<FJsonObject> FSetViewportRealtimeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> EnabledProp = MakeShared<FJsonObject>();
	EnabledProp->SetStringField(TEXT("type"), TEXT("boolean"));
	Properties->SetObjectField(TEXT("enabled"), EnabledProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("enabled")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetViewportRealtimeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	bool bEnabled = false;
	if (!Arguments.IsValid() || !Arguments->TryGetBoolField(TEXT("enabled"), bEnabled))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: enabled"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSimpleResult ViewResult = ViewportModule.SetViewportRealtime(bEnabled);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ViewResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), ViewResult.Message);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set viewport realtime: %s"), *ViewResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
