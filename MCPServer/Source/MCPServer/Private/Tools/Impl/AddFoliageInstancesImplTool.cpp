// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddFoliageInstancesImplTool.h"
#include "Modules/Interfaces/IFoliageModule.h"
#include "Dom/JsonValue.h"

FAddFoliageInstancesImplTool::FAddFoliageInstancesImplTool(IFoliageModule& InFoliageModule)
	: FoliageModule(InFoliageModule)
{
}

FString FAddFoliageInstancesImplTool::GetName() const
{
	return TEXT("add_foliage_instances");
}

FString FAddFoliageInstancesImplTool::GetDescription() const
{
	return TEXT("Add foliage instances at specified transforms for a given static mesh");
}

TSharedPtr<FJsonObject> FAddFoliageInstancesImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// mesh_path
	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"),
		TEXT("Asset path of the static mesh to use as foliage (e.g. /Game/Meshes/MyTree)"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	// transforms
	TSharedPtr<FJsonObject> TransformsProp = MakeShared<FJsonObject>();
	TransformsProp->SetStringField(TEXT("type"), TEXT("array"));
	TransformsProp->SetStringField(TEXT("description"),
		TEXT("Array of transforms for each instance"));

	// transforms items
	TSharedPtr<FJsonObject> TransformItem = MakeShared<FJsonObject>();
	TransformItem->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> TransformProperties = MakeShared<FJsonObject>();

	// location
	TSharedPtr<FJsonObject> LocationProp = MakeShared<FJsonObject>();
	LocationProp->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> LocationProperties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));
	LocationProperties->SetObjectField(TEXT("x"), NumProp);
	LocationProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	LocationProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	LocationProp->SetObjectField(TEXT("properties"), LocationProperties);
	TransformProperties->SetObjectField(TEXT("location"), LocationProp);

	// rotation
	TSharedPtr<FJsonObject> RotationProp = MakeShared<FJsonObject>();
	RotationProp->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> RotationProperties = MakeShared<FJsonObject>();
	RotationProperties->SetObjectField(TEXT("pitch"), MakeShared<FJsonObject>(*NumProp));
	RotationProperties->SetObjectField(TEXT("yaw"), MakeShared<FJsonObject>(*NumProp));
	RotationProperties->SetObjectField(TEXT("roll"), MakeShared<FJsonObject>(*NumProp));
	RotationProp->SetObjectField(TEXT("properties"), RotationProperties);
	TransformProperties->SetObjectField(TEXT("rotation"), RotationProp);

	// scale
	TSharedPtr<FJsonObject> ScaleProp = MakeShared<FJsonObject>();
	ScaleProp->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> ScaleProperties = MakeShared<FJsonObject>();
	ScaleProperties->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*NumProp));
	ScaleProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	ScaleProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	ScaleProp->SetObjectField(TEXT("properties"), ScaleProperties);
	TransformProperties->SetObjectField(TEXT("scale"), ScaleProp);

	TransformItem->SetObjectField(TEXT("properties"), TransformProperties);
	TransformsProp->SetObjectField(TEXT("items"), TransformItem);
	Properties->SetObjectField(TEXT("transforms"), TransformsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("transforms")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddFoliageInstancesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse mesh_path
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

	// Parse transforms array
	const TArray<TSharedPtr<FJsonValue>>* TransformsArray;
	if (!Arguments->TryGetArrayField(TEXT("transforms"), TransformsArray))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: transforms"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	TArray<FTransform> Transforms;
	for (const TSharedPtr<FJsonValue>& TransformValue : *TransformsArray)
	{
		const TSharedPtr<FJsonObject>* TransformObj;
		if (!TransformValue->TryGetObject(TransformObj))
		{
			continue;
		}

		FVector Location = FVector::ZeroVector;
		const TSharedPtr<FJsonObject>* LocationObj;
		if ((*TransformObj)->TryGetObjectField(TEXT("location"), LocationObj))
		{
			(*LocationObj)->TryGetNumberField(TEXT("x"), Location.X);
			(*LocationObj)->TryGetNumberField(TEXT("y"), Location.Y);
			(*LocationObj)->TryGetNumberField(TEXT("z"), Location.Z);
		}

		FRotator Rotation = FRotator::ZeroRotator;
		const TSharedPtr<FJsonObject>* RotationObj;
		if ((*TransformObj)->TryGetObjectField(TEXT("rotation"), RotationObj))
		{
			(*RotationObj)->TryGetNumberField(TEXT("pitch"), Rotation.Pitch);
			(*RotationObj)->TryGetNumberField(TEXT("yaw"), Rotation.Yaw);
			(*RotationObj)->TryGetNumberField(TEXT("roll"), Rotation.Roll);
		}

		FVector Scale = FVector::OneVector;
		const TSharedPtr<FJsonObject>* ScaleObj;
		if ((*TransformObj)->TryGetObjectField(TEXT("scale"), ScaleObj))
		{
			(*ScaleObj)->TryGetNumberField(TEXT("x"), Scale.X);
			(*ScaleObj)->TryGetNumberField(TEXT("y"), Scale.Y);
			(*ScaleObj)->TryGetNumberField(TEXT("z"), Scale.Z);
		}

		FTransform Transform;
		Transform.SetLocation(Location);
		Transform.SetRotation(FQuat(Rotation));
		Transform.SetScale3D(Scale);
		Transforms.Add(Transform);
	}

	FFoliageAddInstancesResult AddResult = FoliageModule.AddFoliageInstances(MeshPath, Transforms);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (AddResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Added %d foliage instances for mesh: %s"), AddResult.InstancesAdded, *MeshPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add foliage instances: %s"), *AddResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
