// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetStaticMeshLodImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FSetStaticMeshLodImplTool::FSetStaticMeshLodImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FSetStaticMeshLodImplTool::GetName() const
{
	return TEXT("set_static_mesh_lod");
}

FString FSetStaticMeshLodImplTool::GetDescription() const
{
	return TEXT("Set LOD screen size thresholds for a static mesh");
}

TSharedPtr<FJsonObject> FSetStaticMeshLodImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the static mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	TSharedPtr<FJsonObject> ScreenSizesProp = MakeShared<FJsonObject>();
	ScreenSizesProp->SetStringField(TEXT("type"), TEXT("array"));
	ScreenSizesProp->SetStringField(TEXT("description"),
		TEXT("Screen size thresholds per LOD, e.g. [1.0, 0.5, 0.25]"));
	TSharedPtr<FJsonObject> ItemsProp = MakeShared<FJsonObject>();
	ItemsProp->SetStringField(TEXT("type"), TEXT("number"));
	ScreenSizesProp->SetObjectField(TEXT("items"), ItemsProp);
	Properties->SetObjectField(TEXT("screen_sizes"), ScreenSizesProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("screen_sizes")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetStaticMeshLodImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	const TArray<TSharedPtr<FJsonValue>>* ScreenSizesArray;
	if (!Arguments->TryGetArrayField(TEXT("screen_sizes"), ScreenSizesArray))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: screen_sizes"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	TArray<float> ScreenSizes;
	for (const TSharedPtr<FJsonValue>& Value : *ScreenSizesArray)
	{
		double NumValue = 0.0;
		if (Value->TryGetNumber(NumValue))
		{
			ScreenSizes.Add(static_cast<float>(NumValue));
		}
	}

	FSetStaticMeshLodResult LodResult = StaticMeshModule.SetStaticMeshLod(MeshPath, ScreenSizes);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LodResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("LOD screen sizes updated for '%s' with %d LOD level(s)."),
			*MeshPath, ScreenSizes.Num());
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set LOD screen sizes: %s"), *LodResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
