// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ReimportSkeletalMeshImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FReimportSkeletalMeshImplTool::FReimportSkeletalMeshImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FReimportSkeletalMeshImplTool::GetName() const
{
	return TEXT("reimport_skeletal_mesh");
}

FString FReimportSkeletalMeshImplTool::GetDescription() const
{
	return TEXT("Reimport a skeletal mesh from its original source file");
}

TSharedPtr<FJsonObject> FReimportSkeletalMeshImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the skeletal mesh to reimport"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FReimportSkeletalMeshImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FReimportSkeletalMeshResult ReimportResult = SkeletalMeshModule.ReimportSkeletalMesh(MeshPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ReimportResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Skeletal mesh '%s' reimported successfully."), *MeshPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to reimport skeletal mesh: %s"), *ReimportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
