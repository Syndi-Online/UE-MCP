// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RegenerateSkeletalLodImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FRegenerateSkeletalLodImplTool::FRegenerateSkeletalLodImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FRegenerateSkeletalLodImplTool::GetName() const
{
	return TEXT("regenerate_skeletal_lod");
}

FString FRegenerateSkeletalLodImplTool::GetDescription() const
{
	return TEXT("Regenerate LOD levels for a skeletal mesh using mesh reduction");
}

TSharedPtr<FJsonObject> FRegenerateSkeletalLodImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the skeletal mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	TSharedPtr<FJsonObject> LodCountProp = MakeShared<FJsonObject>();
	LodCountProp->SetStringField(TEXT("type"), TEXT("integer"));
	LodCountProp->SetStringField(TEXT("description"), TEXT("Number of LOD levels to generate (0 = use default)"));
	Properties->SetObjectField(TEXT("lod_count"), LodCountProp);

	TSharedPtr<FJsonObject> RegenerateProp = MakeShared<FJsonObject>();
	RegenerateProp->SetStringField(TEXT("type"), TEXT("boolean"));
	RegenerateProp->SetStringField(TEXT("description"), TEXT("Regenerate even if LODs were imported"));
	Properties->SetObjectField(TEXT("regenerate_even_if_imported"), RegenerateProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FRegenerateSkeletalLodImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	int32 LodCount = 0;
	if (Arguments->HasField(TEXT("lod_count")))
	{
		LodCount = static_cast<int32>(Arguments->GetNumberField(TEXT("lod_count")));
	}

	bool bRegenerate = false;
	if (Arguments->HasField(TEXT("regenerate_even_if_imported")))
	{
		bRegenerate = Arguments->GetBoolField(TEXT("regenerate_even_if_imported"));
	}

	FRegenerateSkeletalLodResult LodResult = SkeletalMeshModule.RegenerateSkeletalLod(MeshPath, LodCount, bRegenerate);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LodResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Skeletal mesh LOD regenerated for '%s'. New LOD count: %d."),
			*MeshPath, LodResult.NewLodCount);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to regenerate skeletal LOD: %s"), *LodResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
