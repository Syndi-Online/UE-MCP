// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DuplicateAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FDuplicateAssetImplTool::FDuplicateAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FDuplicateAssetImplTool::GetName() const
{
	return TEXT("duplicate_asset");
}

FString FDuplicateAssetImplTool::GetDescription() const
{
	return TEXT("Duplicate an asset to a new path.");
}

TSharedPtr<FJsonObject> FDuplicateAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> SourcePathProp = MakeShared<FJsonObject>();
	SourcePathProp->SetStringField(TEXT("type"), TEXT("string"));
	SourcePathProp->SetStringField(TEXT("description"), TEXT("Source asset path to duplicate"));
	Properties->SetObjectField(TEXT("source_path"), SourcePathProp);

	TSharedPtr<FJsonObject> DestPathProp = MakeShared<FJsonObject>();
	DestPathProp->SetStringField(TEXT("type"), TEXT("string"));
	DestPathProp->SetStringField(TEXT("description"), TEXT("Destination path for the duplicated asset"));
	Properties->SetObjectField(TEXT("destination_path"), DestPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("source_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("destination_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FDuplicateAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString SourcePath;
	FString DestinationPath;
	if (!Arguments.IsValid()
		|| !Arguments->TryGetStringField(TEXT("source_path"), SourcePath)
		|| !Arguments->TryGetStringField(TEXT("destination_path"), DestinationPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: source_path and destination_path are required"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetDuplicateResult DuplicateResult = AssetModule.DuplicateAsset(SourcePath, DestinationPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DuplicateResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Asset duplicated successfully.\nAssetName: %s\nAssetPath: %s"),
			*DuplicateResult.AssetName, *DuplicateResult.AssetPath);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to duplicate asset: %s"), *DuplicateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
