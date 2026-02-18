// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SaveAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FSaveAssetImplTool::FSaveAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FSaveAssetImplTool::GetName() const
{
	return TEXT("save_asset");
}

FString FSaveAssetImplTool::GetDescription() const
{
	return TEXT("Save an asset to disk.");
}

TSharedPtr<FJsonObject> FSaveAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Path of the asset to save"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSaveAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FAssetSaveResult SaveResult = AssetModule.SaveAsset(AssetPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SaveResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Asset saved successfully: %s"), *SaveResult.AssetPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to save asset: %s"), *SaveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
