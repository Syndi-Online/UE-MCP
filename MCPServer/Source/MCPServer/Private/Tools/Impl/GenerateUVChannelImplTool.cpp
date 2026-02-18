// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GenerateUVChannelImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FGenerateUVChannelImplTool::FGenerateUVChannelImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FGenerateUVChannelImplTool::GetName() const
{
	return TEXT("generate_uv_channel");
}

FString FGenerateUVChannelImplTool::GetDescription() const
{
	return TEXT("Generate UV coordinates using a projection method");
}

TSharedPtr<FJsonObject> FGenerateUVChannelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the static mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	TSharedPtr<FJsonObject> LodIndexProp = MakeShared<FJsonObject>();
	LodIndexProp->SetStringField(TEXT("type"), TEXT("integer"));
	LodIndexProp->SetStringField(TEXT("description"), TEXT("LOD index (default 0)"));
	LodIndexProp->SetNumberField(TEXT("default"), 0);
	Properties->SetObjectField(TEXT("lod_index"), LodIndexProp);

	TSharedPtr<FJsonObject> UVChannelIndexProp = MakeShared<FJsonObject>();
	UVChannelIndexProp->SetStringField(TEXT("type"), TEXT("integer"));
	UVChannelIndexProp->SetStringField(TEXT("description"), TEXT("UV channel index to generate into"));
	Properties->SetObjectField(TEXT("uv_channel_index"), UVChannelIndexProp);

	TSharedPtr<FJsonObject> ProjectionTypeProp = MakeShared<FJsonObject>();
	ProjectionTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	ProjectionTypeProp->SetStringField(TEXT("description"),
		TEXT("Projection type: Planar, Cylindrical, Box"));
	Properties->SetObjectField(TEXT("projection_type"), ProjectionTypeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("uv_channel_index")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("projection_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGenerateUVChannelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	double LodIndexValue = 0.0;
	Arguments->TryGetNumberField(TEXT("lod_index"), LodIndexValue);
	int32 LodIndex = static_cast<int32>(LodIndexValue);

	double UVChannelIndexValue = 0.0;
	if (!Arguments->TryGetNumberField(TEXT("uv_channel_index"), UVChannelIndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: uv_channel_index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}
	int32 UVChannelIndex = static_cast<int32>(UVChannelIndexValue);

	FString ProjectionType;
	if (!Arguments->TryGetStringField(TEXT("projection_type"), ProjectionType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: projection_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FUVChannelResult UVResult = StaticMeshModule.GenerateUVChannel(MeshPath, LodIndex, UVChannelIndex, ProjectionType);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (UVResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("UV channel %d generated for '%s' at LOD %d using %s projection.\nNumUVChannels: %d"),
			UVChannelIndex, *MeshPath, LodIndex, *ProjectionType, UVResult.NumUVChannels);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to generate UV channel: %s"), *UVResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
