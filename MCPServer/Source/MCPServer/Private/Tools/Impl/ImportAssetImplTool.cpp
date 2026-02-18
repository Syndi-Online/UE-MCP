// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ImportAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FImportAssetImplTool::FImportAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FImportAssetImplTool::GetName() const
{
	return TEXT("import_asset");
}

FString FImportAssetImplTool::GetDescription() const
{
	return TEXT("Import a file (FBX, texture, audio, etc.) as an asset.");
}

TSharedPtr<FJsonObject> FImportAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> FilePathProp = MakeShared<FJsonObject>();
	FilePathProp->SetStringField(TEXT("type"), TEXT("string"));
	FilePathProp->SetStringField(TEXT("description"),
		TEXT("Absolute file path on disk to import"));
	Properties->SetObjectField(TEXT("file_path"), FilePathProp);

	TSharedPtr<FJsonObject> DestPathProp = MakeShared<FJsonObject>();
	DestPathProp->SetStringField(TEXT("type"), TEXT("string"));
	DestPathProp->SetStringField(TEXT("description"),
		TEXT("Content path destination (e.g. '/Game/Meshes')"));
	Properties->SetObjectField(TEXT("destination_path"), DestPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("file_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("destination_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FImportAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString FilePath;
	FString DestinationPath;
	if (!Arguments.IsValid()
		|| !Arguments->TryGetStringField(TEXT("file_path"), FilePath)
		|| !Arguments->TryGetStringField(TEXT("destination_path"), DestinationPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: file_path and destination_path are required"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetImportResult ImportResult = AssetModule.ImportAsset(FilePath, DestinationPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ImportResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Import successful. %d asset(s) imported:\n"), ImportResult.ImportedAssets.Num());
		for (const FString& ImportedPath : ImportResult.ImportedAssets)
		{
			ResponseText += FString::Printf(TEXT("- %s\n"), *ImportedPath);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to import asset: %s"), *ImportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
