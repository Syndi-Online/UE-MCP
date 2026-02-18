// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetViewportCameraImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FSetViewportCameraImplTool::FSetViewportCameraImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FSetViewportCameraImplTool::GetName() const
{
	return TEXT("set_viewport_camera");
}

FString FSetViewportCameraImplTool::GetDescription() const
{
	return TEXT("Set the viewport camera position and rotation.");
}

TSharedPtr<FJsonObject> FSetViewportCameraImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));

	// Location
	TSharedPtr<FJsonObject> LocationProp = MakeShared<FJsonObject>();
	LocationProp->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> LocProps = MakeShared<FJsonObject>();
	LocProps->SetObjectField(TEXT("x"), NumProp);
	LocProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	LocProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	LocationProp->SetObjectField(TEXT("properties"), LocProps);
	Properties->SetObjectField(TEXT("location"), LocationProp);

	// Rotation
	TSharedPtr<FJsonObject> RotationProp = MakeShared<FJsonObject>();
	RotationProp->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> RotProps = MakeShared<FJsonObject>();
	RotProps->SetObjectField(TEXT("pitch"), MakeShared<FJsonObject>(*NumProp));
	RotProps->SetObjectField(TEXT("yaw"), MakeShared<FJsonObject>(*NumProp));
	RotProps->SetObjectField(TEXT("roll"), MakeShared<FJsonObject>(*NumProp));
	RotationProp->SetObjectField(TEXT("properties"), RotProps);
	Properties->SetObjectField(TEXT("rotation"), RotationProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("location")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("rotation")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetViewportCameraImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	if (!Arguments.IsValid())
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: location, rotation"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse location
	FVector Location = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* LocationObj;
	if (Arguments->TryGetObjectField(TEXT("location"), LocationObj))
	{
		(*LocationObj)->TryGetNumberField(TEXT("x"), Location.X);
		(*LocationObj)->TryGetNumberField(TEXT("y"), Location.Y);
		(*LocationObj)->TryGetNumberField(TEXT("z"), Location.Z);
	}
	else
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: location"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse rotation
	FRotator Rotation = FRotator::ZeroRotator;
	const TSharedPtr<FJsonObject>* RotationObj;
	if (Arguments->TryGetObjectField(TEXT("rotation"), RotationObj))
	{
		(*RotationObj)->TryGetNumberField(TEXT("pitch"), Rotation.Pitch);
		(*RotationObj)->TryGetNumberField(TEXT("yaw"), Rotation.Yaw);
		(*RotationObj)->TryGetNumberField(TEXT("roll"), Rotation.Roll);
	}
	else
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: rotation"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetViewportCameraResult CameraResult = ViewportModule.SetViewportCamera(Location, Rotation);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CameraResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Camera set to Location: (%.3f, %.3f, %.3f), Rotation: (Pitch=%.3f, Yaw=%.3f, Roll=%.3f)"),
			CameraResult.Location.X, CameraResult.Location.Y, CameraResult.Location.Z,
			CameraResult.Rotation.Pitch, CameraResult.Rotation.Yaw, CameraResult.Rotation.Roll);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set viewport camera: %s"), *CameraResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
