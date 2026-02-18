// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetMeshBoundsImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FGetMeshBoundsImplTool::FGetMeshBoundsImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FGetMeshBoundsImplTool::GetName() const
{
	return TEXT("get_mesh_bounds");
}

FString FGetMeshBoundsImplTool::GetDescription() const
{
	return TEXT("Get the bounding box of a static mesh");
}

TSharedPtr<FJsonObject> FGetMeshBoundsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the static mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetMeshBoundsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FMeshBoundsResult BoundsResult = StaticMeshModule.GetMeshBounds(MeshPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BoundsResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Bounds for '%s':\nOrigin: (%.3f, %.3f, %.3f)\nBoxExtent: (%.3f, %.3f, %.3f)\nSphereRadius: %.3f"),
			*MeshPath,
			BoundsResult.Origin.X, BoundsResult.Origin.Y, BoundsResult.Origin.Z,
			BoundsResult.BoxExtent.X, BoundsResult.BoxExtent.Y, BoundsResult.BoxExtent.Z,
			BoundsResult.SphereRadius);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get mesh bounds: %s"), *BoundsResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
