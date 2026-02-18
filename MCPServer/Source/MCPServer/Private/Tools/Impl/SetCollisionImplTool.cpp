// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetCollisionImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FSetCollisionImplTool::FSetCollisionImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FSetCollisionImplTool::GetName() const
{
	return TEXT("set_collision");
}

FString FSetCollisionImplTool::GetDescription() const
{
	return TEXT("Set collision type for a static mesh");
}

TSharedPtr<FJsonObject> FSetCollisionImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the static mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	TSharedPtr<FJsonObject> CollisionTypeProp = MakeShared<FJsonObject>();
	CollisionTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	CollisionTypeProp->SetStringField(TEXT("description"),
		TEXT("Collision type: Box, Sphere, Capsule, NDOP10_X, NDOP10_Y, NDOP10_Z, NDOP18, NDOP26, ComplexAsSimple, UseSimpleAsComplex, RemoveAll"));
	Properties->SetObjectField(TEXT("collision_type"), CollisionTypeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("collision_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetCollisionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString MeshPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("mesh_path"), MeshPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: mesh_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString CollisionType;
	if (!Arguments->TryGetStringField(TEXT("collision_type"), CollisionType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: collision_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetCollisionResult CollisionResult = StaticMeshModule.SetCollision(MeshPath, CollisionType);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CollisionResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Collision set to '%s' for '%s'."),
			*CollisionType, *MeshPath);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set collision: %s"), *CollisionResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
