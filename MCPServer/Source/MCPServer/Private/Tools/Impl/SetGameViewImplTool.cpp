// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetGameViewImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FSetGameViewImplTool::FSetGameViewImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FSetGameViewImplTool::GetName() const
{
	return TEXT("set_game_view");
}

FString FSetGameViewImplTool::GetDescription() const
{
	return TEXT("Toggle Game View mode in the viewport.");
}

TSharedPtr<FJsonObject> FSetGameViewImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FSetGameViewImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FSimpleResult ViewResult = ViewportModule.SetGameView(bEnabled);

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
			FString::Printf(TEXT("Failed to set game view: %s"), *ViewResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
