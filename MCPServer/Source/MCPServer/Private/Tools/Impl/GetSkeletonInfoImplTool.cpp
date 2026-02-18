// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetSkeletonInfoImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FGetSkeletonInfoImplTool::FGetSkeletonInfoImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FGetSkeletonInfoImplTool::GetName() const
{
	return TEXT("get_skeleton_info");
}

FString FGetSkeletonInfoImplTool::GetDescription() const
{
	return TEXT("Get skeleton information: bone hierarchy, vertex count per LOD");
}

TSharedPtr<FJsonObject> FGetSkeletonInfoImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FGetSkeletonInfoImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FSkeletonInfoResult InfoResult = SkeletalMeshModule.GetSkeletonInfo(MeshPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (InfoResult.bSuccess)
	{
		TSharedPtr<FJsonObject> InfoJson = MakeShared<FJsonObject>();
		InfoJson->SetStringField(TEXT("skeleton"), InfoResult.SkeletonName);
		InfoJson->SetNumberField(TEXT("num_bones"), InfoResult.NumBones);
		InfoJson->SetNumberField(TEXT("num_raw_bones"), InfoResult.NumRawBones);
		InfoJson->SetNumberField(TEXT("lod_count"), InfoResult.LodCount);

		TArray<TSharedPtr<FJsonValue>> VertexCounts;
		for (int32 Count : InfoResult.VertexCountPerLod)
		{
			VertexCounts.Add(MakeShared<FJsonValueNumber>(Count));
		}
		InfoJson->SetArrayField(TEXT("vertex_count_per_lod"), VertexCounts);

		TArray<TSharedPtr<FJsonValue>> BonesArray;
		for (const FBoneInfo& Bone : InfoResult.Bones)
		{
			TSharedPtr<FJsonObject> BoneJson = MakeShared<FJsonObject>();
			BoneJson->SetNumberField(TEXT("index"), Bone.BoneIndex);
			BoneJson->SetStringField(TEXT("name"), Bone.BoneName);
			BoneJson->SetNumberField(TEXT("parent_index"), Bone.ParentIndex);
			BonesArray.Add(MakeShared<FJsonValueObject>(BoneJson));
		}
		InfoJson->SetArrayField(TEXT("bones"), BonesArray);

		FString ResponseText;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseText);
		FJsonSerializer::Serialize(InfoJson.ToSharedRef(), Writer);

		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get skeleton info: %s"), *InfoResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
