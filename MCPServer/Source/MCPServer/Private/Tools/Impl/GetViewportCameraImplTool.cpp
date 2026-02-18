// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetViewportCameraImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FGetViewportCameraImplTool::FGetViewportCameraImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FGetViewportCameraImplTool::GetName() const
{
	return TEXT("get_viewport_camera");
}

FString FGetViewportCameraImplTool::GetDescription() const
{
	return TEXT("Get the current viewport camera position, rotation, and FOV.");
}

TSharedPtr<FJsonObject> FGetViewportCameraImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FGetViewportCameraImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FViewportCameraResult CameraResult = ViewportModule.GetViewportCamera();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CameraResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Location: (%.3f, %.3f, %.3f)\nRotation: (Pitch=%.3f, Yaw=%.3f, Roll=%.3f)\nFOV: %.1f"),
			CameraResult.Location.X, CameraResult.Location.Y, CameraResult.Location.Z,
			CameraResult.Rotation.Pitch, CameraResult.Rotation.Yaw, CameraResult.Rotation.Roll,
			CameraResult.FOV);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get viewport camera: %s"), *CameraResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
