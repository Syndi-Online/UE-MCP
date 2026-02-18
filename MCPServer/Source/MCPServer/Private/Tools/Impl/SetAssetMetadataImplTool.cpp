// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetAssetMetadataImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FSetAssetMetadataImplTool::FSetAssetMetadataImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FSetAssetMetadataImplTool::GetName() const
{
	return TEXT("set_asset_metadata");
}

FString FSetAssetMetadataImplTool::GetDescription() const
{
	return TEXT("Set a metadata tag on an asset.");
}

TSharedPtr<FJsonObject> FSetAssetMetadataImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path to set metadata on"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	TSharedPtr<FJsonObject> TagNameProp = MakeShared<FJsonObject>();
	TagNameProp->SetStringField(TEXT("type"), TEXT("string"));
	TagNameProp->SetStringField(TEXT("description"), TEXT("Name of the metadata tag"));
	Properties->SetObjectField(TEXT("tag_name"), TagNameProp);

	TSharedPtr<FJsonObject> TagValueProp = MakeShared<FJsonObject>();
	TagValueProp->SetStringField(TEXT("type"), TEXT("string"));
	TagValueProp->SetStringField(TEXT("description"), TEXT("Value of the metadata tag"));
	Properties->SetObjectField(TEXT("tag_value"), TagValueProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("tag_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("tag_value")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetAssetMetadataImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetPath;
	FString TagName;
	FString TagValue;
	if (!Arguments.IsValid()
		|| !Arguments->TryGetStringField(TEXT("asset_path"), AssetPath)
		|| !Arguments->TryGetStringField(TEXT("tag_name"), TagName)
		|| !Arguments->TryGetStringField(TEXT("tag_value"), TagValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: asset_path, tag_name, and tag_value are all required"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetSetMetadataResult SetResult = AssetModule.SetAssetMetadata(AssetPath, TagName, TagValue);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Metadata tag '%s' set to '%s' on asset '%s'."), *TagName, *TagValue, *AssetPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set asset metadata: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
