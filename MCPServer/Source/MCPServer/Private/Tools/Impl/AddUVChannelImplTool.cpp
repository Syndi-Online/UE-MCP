// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddUVChannelImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FAddUVChannelImplTool::FAddUVChannelImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FAddUVChannelImplTool::GetName() const
{
	return TEXT("add_uv_channel");
}

FString FAddUVChannelImplTool::GetDescription() const
{
	return TEXT("Add a UV channel to a static mesh LOD");
}

TSharedPtr<FJsonObject> FAddUVChannelImplTool::GetInputSchema() const
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

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddUVChannelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FUVChannelResult UVResult = StaticMeshModule.AddUVChannel(MeshPath, LodIndex);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (UVResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("UV channel added to '%s' at LOD %d.\nNumUVChannels: %d"),
			*MeshPath, LodIndex, UVResult.NumUVChannels);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add UV channel: %s"), *UVResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
