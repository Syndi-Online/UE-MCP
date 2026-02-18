// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetActorTransformImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FGetActorTransformImplTool::FGetActorTransformImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FGetActorTransformImplTool::GetName() const
{
	return TEXT("get_actor_transform");
}

FString FGetActorTransformImplTool::GetDescription() const
{
	return TEXT("Get the transform (location, rotation, scale) of an actor.");
}

TSharedPtr<FJsonObject> FGetActorTransformImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"),
		TEXT("Actor name, label, or path"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetActorTransformImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FActorTransformResult TransformResult = ActorModule.GetActorTransform(ActorIdentifier);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (TransformResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Transform for '%s':\nLocation: (%.3f, %.3f, %.3f)\nRotation: (Pitch=%.3f, Yaw=%.3f, Roll=%.3f)\nScale: (%.3f, %.3f, %.3f)"),
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
			FString::Printf(TEXT("Failed to get transform: %s"), *TransformResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
