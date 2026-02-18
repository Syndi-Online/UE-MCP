// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetAssetMetadataImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FGetAssetMetadataImplTool::FGetAssetMetadataImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FGetAssetMetadataImplTool::GetName() const
{
	return TEXT("get_asset_metadata");
}

FString FGetAssetMetadataImplTool::GetDescription() const
{
	return TEXT("Get metadata tags from an asset.");
}

TSharedPtr<FJsonObject> FGetAssetMetadataImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path to get metadata from"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetAssetMetadataImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FAssetMetadataResult MetadataResult = AssetModule.GetAssetMetadata(AssetPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (MetadataResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Metadata for '%s' (%d tags):\n"), *AssetPath, MetadataResult.Tags.Num());
		for (const auto& Pair : MetadataResult.Tags)
		{
			ResponseText += FString::Printf(TEXT("- %s = %s\n"), *Pair.Key, *Pair.Value);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get asset metadata: %s"), *MetadataResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
