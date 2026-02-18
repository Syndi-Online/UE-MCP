// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetActorTransformImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FSetActorTransformImplTool::FSetActorTransformImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FSetActorTransformImplTool::GetName() const
{
	return TEXT("set_actor_transform");
}

FString FSetActorTransformImplTool::GetDescription() const
{
	return TEXT("Set the transform (location, rotation, scale) of an actor. Only provided components are changed.");
}

TSharedPtr<FJsonObject> FSetActorTransformImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"),
		TEXT("Actor name, label, or path"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	// Location
	TSharedPtr<FJsonObject> LocationProp = MakeShared<FJsonObject>();
	LocationProp->SetStringField(TEXT("type"), TEXT("object"));
	LocationProp->SetStringField(TEXT("description"), TEXT("New location {x, y, z}. Optional."));
	TSharedPtr<FJsonObject> LocProps = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));
	LocProps->SetObjectField(TEXT("x"), NumProp);
	LocProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	LocProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	LocationProp->SetObjectField(TEXT("properties"), LocProps);
	Properties->SetObjectField(TEXT("location"), LocationProp);

	// Rotation
	TSharedPtr<FJsonObject> RotationProp = MakeShared<FJsonObject>();
	RotationProp->SetStringField(TEXT("type"), TEXT("object"));
	RotationProp->SetStringField(TEXT("description"), TEXT("New rotation {pitch, yaw, roll} in degrees. Optional."));
	TSharedPtr<FJsonObject> RotProps = MakeShared<FJsonObject>();
	RotProps->SetObjectField(TEXT("pitch"), MakeShared<FJsonObject>(*NumProp));
	RotProps->SetObjectField(TEXT("yaw"), MakeShared<FJsonObject>(*NumProp));
	RotProps->SetObjectField(TEXT("roll"), MakeShared<FJsonObject>(*NumProp));
	RotationProp->SetObjectField(TEXT("properties"), RotProps);
	Properties->SetObjectField(TEXT("rotation"), RotationProp);

	// Scale
	TSharedPtr<FJsonObject> ScaleProp = MakeShared<FJsonObject>();
	ScaleProp->SetStringField(TEXT("type"), TEXT("object"));
	ScaleProp->SetStringField(TEXT("description"), TEXT("New scale {x, y, z}. Optional."));
	TSharedPtr<FJsonObject> ScaleProps = MakeShared<FJsonObject>();
	ScaleProps->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*NumProp));
	ScaleProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	ScaleProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	ScaleProp->SetObjectField(TEXT("properties"), ScaleProps);
	Properties->SetObjectField(TEXT("scale"), ScaleProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetActorTransformImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_identifier"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional location
	FVector Location;
	FVector* LocationPtr = nullptr;
	const TSharedPtr<FJsonObject>* LocationObj;
	if (Arguments->TryGetObjectField(TEXT("location"), LocationObj))
	{
		Location = FVector::ZeroVector;
		(*LocationObj)->TryGetNumberField(TEXT("x"), Location.X);
		(*LocationObj)->TryGetNumberField(TEXT("y"), Location.Y);
		(*LocationObj)->TryGetNumberField(TEXT("z"), Location.Z);
		LocationPtr = &Location;
	}

	// Parse optional rotation
	FRotator Rotation;
	FRotator* RotationPtr = nullptr;
	const TSharedPtr<FJsonObject>* RotationObj;
	if (Arguments->TryGetObjectField(TEXT("rotation"), RotationObj))
	{
		Rotation = FRotator::ZeroRotator;
		(*RotationObj)->TryGetNumberField(TEXT("pitch"), Rotation.Pitch);
		(*RotationObj)->TryGetNumberField(TEXT("yaw"), Rotation.Yaw);
		(*RotationObj)->TryGetNumberField(TEXT("roll"), Rotation.Roll);
		RotationPtr = &Rotation;
	}

	// Parse optional scale
	FVector Scale;
	FVector* ScalePtr = nullptr;
	const TSharedPtr<FJsonObject>* ScaleObj;
	if (Arguments->TryGetObjectField(TEXT("scale"), ScaleObj))
	{
		Scale = FVector::OneVector;
		(*ScaleObj)->TryGetNumberField(TEXT("x"), Scale.X);
		(*ScaleObj)->TryGetNumberField(TEXT("y"), Scale.Y);
		(*ScaleObj)->TryGetNumberField(TEXT("z"), Scale.Z);
		ScalePtr = &Scale;
	}

	FActorSetTransformResult TransformResult = ActorModule.SetActorTransform(
		ActorIdentifier, LocationPtr, RotationPtr, ScalePtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (TransformResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Transform updated for '%s':\nLocation: (%.3f, %.3f, %.3f)\nRotation: (Pitch=%.3f, Yaw=%.3f, Roll=%.3f)\nScale: (%.3f, %.3f, %.3f)"),
			*ActorIdentifier,
			TransformResult.Location.X, TransformResult.Location.Y, TransformResult.Location.Z,
			TransformResult.Rotation.Pitch, TransformResult.Rotation.Yaw, TransformResult.Rotation.Roll,
			TransformResult.Scale.X, TransformResult.Scale.Y, TransformResult.Scale.Z);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set transform: %s"), *TransformResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
