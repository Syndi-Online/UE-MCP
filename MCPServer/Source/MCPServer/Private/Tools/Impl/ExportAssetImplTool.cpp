// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ExportAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FExportAssetImplTool::FExportAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FExportAssetImplTool::GetName() const
{
	return TEXT("export_asset");
}

FString FExportAssetImplTool::GetDescription() const
{
	return TEXT("Export an asset to a file on disk.");
}

TSharedPtr<FJsonObject> FExportAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path to export"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	TSharedPtr<FJsonObject> ExportPathProp = MakeShared<FJsonObject>();
	ExportPathProp->SetStringField(TEXT("type"), TEXT("string"));
	ExportPathProp->SetStringField(TEXT("description"),
		TEXT("Absolute directory on disk to export to"));
	Properties->SetObjectField(TEXT("export_path"), ExportPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("export_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FExportAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetPath;
	FString ExportPath;
	if (!Arguments.IsValid()
		|| !Arguments->TryGetStringField(TEXT("asset_path"), AssetPath)
		|| !Arguments->TryGetStringField(TEXT("export_path"), ExportPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: asset_path and export_path are required"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetExportResult ExportResult = AssetModule.ExportAsset(AssetPath, ExportPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ExportResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Asset '%s' exported successfully to '%s'."), *AssetPath, *ExportPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to export asset: %s"), *ExportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
