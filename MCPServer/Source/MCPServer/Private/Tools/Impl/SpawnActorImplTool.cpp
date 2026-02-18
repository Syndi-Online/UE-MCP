// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SpawnActorImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FSpawnActorImplTool::FSpawnActorImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FSpawnActorImplTool::GetName() const
{
	return TEXT("spawn_actor");
}

FString FSpawnActorImplTool::GetDescription() const
{
	return TEXT("Spawn an actor in the editor world from a class path. Returns the spawned actor's name, label, class, location and rotation.");
}

TSharedPtr<FJsonObject> FSpawnActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// actor_class
	TSharedPtr<FJsonObject> ActorClassProp = MakeShared<FJsonObject>();
	ActorClassProp->SetStringField(TEXT("type"), TEXT("string"));
	ActorClassProp->SetStringField(TEXT("description"), TEXT("Full class path, e.g. \"/Script/Engine.StaticMeshActor\" or \"/Script/Engine.PointLight\""));
	Properties->SetObjectField(TEXT("actor_class"), ActorClassProp);

	// location
	TSharedPtr<FJsonObject> LocationProp = MakeShared<FJsonObject>();
	LocationProp->SetStringField(TEXT("type"), TEXT("object"));
	LocationProp->SetStringField(TEXT("description"), TEXT("World location {x, y, z}"));

	TSharedPtr<FJsonObject> LocationProperties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> CoordProp = MakeShared<FJsonObject>();
	CoordProp->SetStringField(TEXT("type"), TEXT("number"));
	LocationProperties->SetObjectField(TEXT("x"), CoordProp);
	LocationProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	LocationProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	LocationProp->SetObjectField(TEXT("properties"), LocationProperties);

	Properties->SetObjectField(TEXT("location"), LocationProp);

	// rotation (optional)
	TSharedPtr<FJsonObject> RotationProp = MakeShared<FJsonObject>();
	RotationProp->SetStringField(TEXT("type"), TEXT("object"));
	RotationProp->SetStringField(TEXT("description"), TEXT("World rotation {pitch, yaw, roll} in degrees. Optional, defaults to zero."));

	TSharedPtr<FJsonObject> RotationProperties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> AngleProp = MakeShared<FJsonObject>();
	AngleProp->SetStringField(TEXT("type"), TEXT("number"));
	RotationProperties->SetObjectField(TEXT("pitch"), AngleProp);
	RotationProperties->SetObjectField(TEXT("yaw"), MakeShared<FJsonObject>(*AngleProp));
	RotationProperties->SetObjectField(TEXT("roll"), MakeShared<FJsonObject>(*AngleProp));
	RotationProp->SetObjectField(TEXT("properties"), RotationProperties);

	Properties->SetObjectField(TEXT("rotation"), RotationProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_class")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("location")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSpawnActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse actor_class
	FString ActorClassPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_class"), ActorClassPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_class"));
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

	// Parse rotation (optional)
	FRotator Rotation = FRotator::ZeroRotator;
	const TSharedPtr<FJsonObject>* RotationObj;
	if (Arguments->TryGetObjectField(TEXT("rotation"), RotationObj))
	{
		(*RotationObj)->TryGetNumberField(TEXT("pitch"), Rotation.Pitch);
		(*RotationObj)->TryGetNumberField(TEXT("yaw"), Rotation.Yaw);
		(*RotationObj)->TryGetNumberField(TEXT("roll"), Rotation.Roll);
	}

	// Call the module
	FActorSpawnResult SpawnResult = ActorModule.SpawnActor(ActorClassPath, Location, Rotation);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SpawnResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Actor spawned successfully.\nName: %s\nLabel: %s\nClass: %s\nLocation: (%.1f, %.1f, %.1f)\nRotation: (Pitch=%.1f, Yaw=%.1f, Roll=%.1f)"),
			*SpawnResult.ActorName,
			*SpawnResult.ActorLabel,
			*SpawnResult.ActorClass,
			SpawnResult.Location.X, SpawnResult.Location.Y, SpawnResult.Location.Z,
			SpawnResult.Rotation.Pitch, SpawnResult.Rotation.Yaw, SpawnResult.Rotation.Roll);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"), FString::Printf(TEXT("Failed to spawn actor: %s"), *SpawnResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
