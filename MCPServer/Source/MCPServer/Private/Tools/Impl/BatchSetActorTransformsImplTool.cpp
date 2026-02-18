// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchSetActorTransformsImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FBatchSetActorTransformsImplTool::FBatchSetActorTransformsImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FBatchSetActorTransformsImplTool::GetName() const
{
	return TEXT("batch_set_actor_transforms");
}

FString FBatchSetActorTransformsImplTool::GetDescription() const
{
	return TEXT("Set transforms (location, rotation, scale) on multiple actors in one call. Up to 100 operations per batch.");
}

TSharedPtr<FJsonObject> FBatchSetActorTransformsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> OpsProp = MakeShared<FJsonObject>();
	OpsProp->SetStringField(TEXT("type"), TEXT("array"));
	OpsProp->SetStringField(TEXT("description"), TEXT("Array of operations. Each has actor_identifier and optional location {x,y,z}, rotation {pitch,yaw,roll}, scale {x,y,z}."));

	TSharedPtr<FJsonObject> ItemSchema = MakeShared<FJsonObject>();
	ItemSchema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> ItemProps = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorProp = MakeShared<FJsonObject>();
	ActorProp->SetStringField(TEXT("type"), TEXT("string"));
	ItemProps->SetObjectField(TEXT("actor_identifier"), ActorProp);

	TSharedPtr<FJsonObject> LocProp = MakeShared<FJsonObject>();
	LocProp->SetStringField(TEXT("type"), TEXT("object"));
	LocProp->SetStringField(TEXT("description"), TEXT("Location {x, y, z}"));
	ItemProps->SetObjectField(TEXT("location"), LocProp);

	TSharedPtr<FJsonObject> RotProp = MakeShared<FJsonObject>();
	RotProp->SetStringField(TEXT("type"), TEXT("object"));
	RotProp->SetStringField(TEXT("description"), TEXT("Rotation {pitch, yaw, roll} in degrees"));
	ItemProps->SetObjectField(TEXT("rotation"), RotProp);

	TSharedPtr<FJsonObject> ScaleProp = MakeShared<FJsonObject>();
	ScaleProp->SetStringField(TEXT("type"), TEXT("object"));
	ScaleProp->SetStringField(TEXT("description"), TEXT("Scale {x, y, z}"));
	ItemProps->SetObjectField(TEXT("scale"), ScaleProp);

	ItemSchema->SetObjectField(TEXT("properties"), ItemProps);
	OpsProp->SetObjectField(TEXT("items"), ItemSchema);
	Properties->SetObjectField(TEXT("operations"), OpsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("operations")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchSetActorTransformsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	const TArray<TSharedPtr<FJsonValue>>* Operations = nullptr;
	if (!Arguments.IsValid() || !Arguments->TryGetArrayField(TEXT("operations"), Operations) || !Operations)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: operations (array)"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (Operations->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Too many operations. Maximum 100 per batch."));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	FString Errors;

	for (int32 i = 0; i < Operations->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* OpObj = nullptr;
		if (!(*Operations)[i]->TryGetObject(OpObj) || !OpObj || !(*OpObj).IsValid())
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Invalid operation object\n"), i);
			continue;
		}

		FString ActorIdentifier;
		if (!(*OpObj)->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier))
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Missing actor_identifier\n"), i);
			continue;
		}

		// Parse optional location
		FVector Location;
		FVector* LocationPtr = nullptr;
		const TSharedPtr<FJsonObject>* LocationObj;
		if ((*OpObj)->TryGetObjectField(TEXT("location"), LocationObj))
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
		if ((*OpObj)->TryGetObjectField(TEXT("rotation"), RotationObj))
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
		if ((*OpObj)->TryGetObjectField(TEXT("scale"), ScaleObj))
		{
			Scale = FVector::OneVector;
			(*ScaleObj)->TryGetNumberField(TEXT("x"), Scale.X);
			(*ScaleObj)->TryGetNumberField(TEXT("y"), Scale.Y);
			(*ScaleObj)->TryGetNumberField(TEXT("z"), Scale.Z);
			ScalePtr = &Scale;
		}

		FActorSetTransformResult OpResult = ActorModule.SetActorTransform(
			ActorIdentifier, LocationPtr, RotationPtr, ScalePtr);

		if (OpResult.bSuccess)
		{
			Succeeded++;
		}
		else
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] %s\n"), i, *OpResult.ErrorMessage);
		}
	}

	FString ResponseText = FString::Printf(TEXT("Batch completed: %d succeeded, %d failed"), Succeeded, Failed);
	if (!Errors.IsEmpty())
	{
		ResponseText += TEXT("\n\nErrors:\n") + Errors;
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), ResponseText);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), Succeeded == 0 && Failed > 0);

	return Result;
}
