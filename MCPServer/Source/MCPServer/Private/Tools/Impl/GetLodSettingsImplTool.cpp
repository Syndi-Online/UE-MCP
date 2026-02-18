// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetLodSettingsImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FGetLodSettingsImplTool::FGetLodSettingsImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FGetLodSettingsImplTool::GetName() const
{
	return TEXT("get_lod_settings");
}

FString FGetLodSettingsImplTool::GetDescription() const
{
	return TEXT("Get LOD settings for a static mesh (count, screen sizes, vertex/triangle counts)");
}

TSharedPtr<FJsonObject> FGetLodSettingsImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FGetLodSettingsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FGetLodSettingsResult LodResult = StaticMeshModule.GetLodSettings(MeshPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LodResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("LOD settings for '%s':\nLodCount: %d\nLodGroup: %s"),
			*MeshPath, LodResult.LodCount, *LodResult.LodGroup);

		for (const FLodInfo& Lod : LodResult.Lods)
		{
			ResponseText += FString::Printf(
				TEXT("\n  LOD %d: ScreenSize=%.4f, Vertices=%d, Triangles=%d"),
				Lod.LodIndex, Lod.ScreenSize, Lod.NumVertices, Lod.NumTriangles);
		}

		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get LOD settings: %s"), *LodResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
