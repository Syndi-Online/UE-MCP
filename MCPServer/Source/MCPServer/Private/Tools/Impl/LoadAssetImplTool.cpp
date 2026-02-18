// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/LoadAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FLoadAssetImplTool::FLoadAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FLoadAssetImplTool::GetName() const
{
	return TEXT("load_asset");
}

FString FLoadAssetImplTool::GetDescription() const
{
	return TEXT("Load an asset by its path and return its info.");
}

TSharedPtr<FJsonObject> FLoadAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"),
		TEXT("Asset path (e.g. '/Game/Meshes/MyMesh')"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FLoadAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("asset_path"), AssetPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: asset_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetLoadResult LoadResult = AssetModule.LoadAsset(AssetPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LoadResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Asset loaded successfully.\nAssetName: %s\nAssetPath: %s\nAssetClass: %s"),
			*LoadResult.AssetName, *LoadResult.AssetPath, *LoadResult.AssetClass);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to load asset: %s"), *LoadResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
