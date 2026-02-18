// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DuplicateActorImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FDuplicateActorImplTool::FDuplicateActorImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FDuplicateActorImplTool::GetName() const
{
	return TEXT("duplicate_actor");
}

FString FDuplicateActorImplTool::GetDescription() const
{
	return TEXT("Duplicate an actor in the editor world. The duplicate can be offset from the original position.");
}

TSharedPtr<FJsonObject> FDuplicateActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"),
		TEXT("Actor name, label, or path to duplicate"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	TSharedPtr<FJsonObject> OffsetProp = MakeShared<FJsonObject>();
	OffsetProp->SetStringField(TEXT("type"), TEXT("object"));
	OffsetProp->SetStringField(TEXT("description"),
		TEXT("Translation offset {x, y, z} from the original. Optional, defaults to (0, 0, 0)."));

	TSharedPtr<FJsonObject> OffsetProperties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> CoordProp = MakeShared<FJsonObject>();
	CoordProp->SetStringField(TEXT("type"), TEXT("number"));
	OffsetProperties->SetObjectField(TEXT("x"), CoordProp);
	OffsetProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	OffsetProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	OffsetProp->SetObjectField(TEXT("properties"), OffsetProperties);

	Properties->SetObjectField(TEXT("offset"), OffsetProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FDuplicateActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FVector Offset = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* OffsetObj;
	if (Arguments->TryGetObjectField(TEXT("offset"), OffsetObj))
	{
		(*OffsetObj)->TryGetNumberField(TEXT("x"), Offset.X);
		(*OffsetObj)->TryGetNumberField(TEXT("y"), Offset.Y);
		(*OffsetObj)->TryGetNumberField(TEXT("z"), Offset.Z);
	}

	FActorDuplicateResult DuplicateResult = ActorModule.DuplicateActor(ActorIdentifier, Offset);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DuplicateResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Actor duplicated successfully.\nName: %s\nLabel: %s\nClass: %s\nLocation: (%.1f, %.1f, %.1f)"),
			*DuplicateResult.ActorName,
			*DuplicateResult.ActorLabel,
			*DuplicateResult.ActorClass,
			DuplicateResult.Location.X, DuplicateResult.Location.Y, DuplicateResult.Location.Z);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to duplicate actor: %s"), *DuplicateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
