// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetCameraFovImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FSetCameraFovImplTool::FSetCameraFovImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FSetCameraFovImplTool::GetName() const
{
	return TEXT("set_camera_fov");
}

FString FSetCameraFovImplTool::GetDescription() const
{
	return TEXT("Set the viewport camera field of view.");
}

TSharedPtr<FJsonObject> FSetCameraFovImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> FovProp = MakeShared<FJsonObject>();
	FovProp->SetStringField(TEXT("type"), TEXT("number"));
	FovProp->SetStringField(TEXT("description"), TEXT("FOV in degrees (5-170)"));
	Properties->SetObjectField(TEXT("fov"), FovProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("fov")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetCameraFovImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	double FOV = 0.0;
	if (!Arguments.IsValid() || !Arguments->TryGetNumberField(TEXT("fov"), FOV))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: fov"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSimpleResult ViewResult = ViewportModule.SetCameraFOV(static_cast<float>(FOV));

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
			FString::Printf(TEXT("Failed to set camera FOV: %s"), *ViewResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
