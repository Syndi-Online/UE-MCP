// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreatePhysicsCollisionImplTool.h"
#include "Modules/Interfaces/IPhysicsModule.h"
#include "Dom/JsonValue.h"

FCreatePhysicsCollisionImplTool::FCreatePhysicsCollisionImplTool(IPhysicsModule& InPhysicsModule)
	: PhysicsModule(InPhysicsModule)
{
}

FString FCreatePhysicsCollisionImplTool::GetName() const
{
	return TEXT("create_physics_collision");
}

FString FCreatePhysicsCollisionImplTool::GetDescription() const
{
	return TEXT("Add a collision shape (box, sphere, or capsule) to an actor's static mesh body setup");
}

TSharedPtr<FJsonObject> FCreatePhysicsCollisionImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorNameProp = MakeShared<FJsonObject>();
	ActorNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ActorNameProp->SetStringField(TEXT("description"), TEXT("Name of the actor"));
	Properties->SetObjectField(TEXT("actor_name"), ActorNameProp);

	TSharedPtr<FJsonObject> ShapeTypeProp = MakeShared<FJsonObject>();
	ShapeTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	ShapeTypeProp->SetStringField(TEXT("description"), TEXT("Type of shape: box, sphere, or capsule"));
	Properties->SetObjectField(TEXT("shape_type"), ShapeTypeProp);

	// Dimensions
	TSharedPtr<FJsonObject> DimensionsProp = MakeShared<FJsonObject>();
	DimensionsProp->SetStringField(TEXT("type"), TEXT("object"));
	DimensionsProp->SetStringField(TEXT("description"), TEXT("Shape dimensions (box: half-extents XYZ, sphere: radius in X, capsule: radius in X and length in Y)"));
	TSharedPtr<FJsonObject> DimProps = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));
	DimProps->SetObjectField(TEXT("x"), NumProp);
	DimProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	DimProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	DimensionsProp->SetObjectField(TEXT("properties"), DimProps);
	Properties->SetObjectField(TEXT("dimensions"), DimensionsProp);

	// Offset
	TSharedPtr<FJsonObject> OffsetProp = MakeShared<FJsonObject>();
	OffsetProp->SetStringField(TEXT("type"), TEXT("object"));
	OffsetProp->SetStringField(TEXT("description"), TEXT("Local offset of the shape (defaults to 0,0,0)"));
	TSharedPtr<FJsonObject> OffsetProps = MakeShared<FJsonObject>();
	OffsetProps->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*NumProp));
	OffsetProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	OffsetProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	OffsetProp->SetObjectField(TEXT("properties"), OffsetProps);
	Properties->SetObjectField(TEXT("offset"), OffsetProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("shape_type")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("dimensions")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreatePhysicsCollisionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorName;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_name"), ActorName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ShapeType;
	if (!Arguments->TryGetStringField(TEXT("shape_type"), ShapeType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: shape_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse required dimensions
	FVector Dimensions = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* DimensionsObj;
	if (!Arguments->TryGetObjectField(TEXT("dimensions"), DimensionsObj))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: dimensions"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}
	(*DimensionsObj)->TryGetNumberField(TEXT("x"), Dimensions.X);
	(*DimensionsObj)->TryGetNumberField(TEXT("y"), Dimensions.Y);
	(*DimensionsObj)->TryGetNumberField(TEXT("z"), Dimensions.Z);

	// Parse optional offset
	FVector Offset = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* OffsetObj;
	if (Arguments->TryGetObjectField(TEXT("offset"), OffsetObj))
	{
		(*OffsetObj)->TryGetNumberField(TEXT("x"), Offset.X);
		(*OffsetObj)->TryGetNumberField(TEXT("y"), Offset.Y);
		(*OffsetObj)->TryGetNumberField(TEXT("z"), Offset.Z);
	}

	FPhysicsCollisionResult CollisionResult = PhysicsModule.CreatePhysicsCollision(ActorName, ShapeType, Dimensions, Offset);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CollisionResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Added %s collision shape (%d shape(s) total)"),
			*ShapeType, CollisionResult.ShapesAdded);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create physics collision: %s"), *CollisionResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
