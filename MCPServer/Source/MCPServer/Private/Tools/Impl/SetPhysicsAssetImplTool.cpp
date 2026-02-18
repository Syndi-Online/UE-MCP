// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetPhysicsAssetImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FSetPhysicsAssetImplTool::FSetPhysicsAssetImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FSetPhysicsAssetImplTool::GetName() const
{
	return TEXT("set_physics_asset");
}

FString FSetPhysicsAssetImplTool::GetDescription() const
{
	return TEXT("Set or clear the Physics Asset on a skeletal mesh");
}

TSharedPtr<FJsonObject> FSetPhysicsAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the skeletal mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	TSharedPtr<FJsonObject> PhysPathProp = MakeShared<FJsonObject>();
	PhysPathProp->SetStringField(TEXT("type"), TEXT("string"));
	PhysPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Physics Asset to assign (empty string to clear)"));
	Properties->SetObjectField(TEXT("physics_asset_path"), PhysPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("physics_asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetPhysicsAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString PhysicsAssetPath;
	Arguments->TryGetStringField(TEXT("physics_asset_path"), PhysicsAssetPath);

	FSetPhysicsAssetResult PhysResult = SkeletalMeshModule.SetPhysicsAsset(MeshPath, PhysicsAssetPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PhysResult.bSuccess)
	{
		FString ResponseText;
		if (PhysicsAssetPath.IsEmpty())
		{
			ResponseText = FString::Printf(TEXT("Physics asset cleared from '%s'."), *MeshPath);
		}
		else
		{
			ResponseText = FString::Printf(TEXT("Physics asset '%s' assigned to '%s'."), *PhysicsAssetPath, *MeshPath);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set physics asset: %s"), *PhysResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
