// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetPhysicsAssetImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FGetPhysicsAssetImplTool::FGetPhysicsAssetImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FGetPhysicsAssetImplTool::GetName() const
{
	return TEXT("get_physics_asset");
}

FString FGetPhysicsAssetImplTool::GetDescription() const
{
	return TEXT("Get the Physics Asset assigned to a skeletal mesh");
}

TSharedPtr<FJsonObject> FGetPhysicsAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the skeletal mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetPhysicsAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FGetPhysicsAssetResult PhysResult = SkeletalMeshModule.GetPhysicsAsset(MeshPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PhysResult.bSuccess)
	{
		TSharedPtr<FJsonObject> InfoJson = MakeShared<FJsonObject>();
		InfoJson->SetStringField(TEXT("physics_asset_path"), PhysResult.PhysicsAssetPath);
		InfoJson->SetStringField(TEXT("physics_asset_name"), PhysResult.PhysicsAssetName);
		InfoJson->SetNumberField(TEXT("num_bodies"), PhysResult.NumBodies);
		InfoJson->SetNumberField(TEXT("num_constraints"), PhysResult.NumConstraints);

		FString ResponseText;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseText);
		FJsonSerializer::Serialize(InfoJson.ToSharedRef(), Writer);

		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get physics asset: %s"), *PhysResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
