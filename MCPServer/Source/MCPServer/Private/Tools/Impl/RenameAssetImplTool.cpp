// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RenameAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FRenameAssetImplTool::FRenameAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FRenameAssetImplTool::GetName() const
{
	return TEXT("rename_asset");
}

FString FRenameAssetImplTool::GetDescription() const
{
	return TEXT("Rename or move an asset to a new path.");
}

TSharedPtr<FJsonObject> FRenameAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> SourcePathProp = MakeShared<FJsonObject>();
	SourcePathProp->SetStringField(TEXT("type"), TEXT("string"));
	SourcePathProp->SetStringField(TEXT("description"), TEXT("Current asset path"));
	Properties->SetObjectField(TEXT("source_path"), SourcePathProp);

	TSharedPtr<FJsonObject> DestPathProp = MakeShared<FJsonObject>();
	DestPathProp->SetStringField(TEXT("type"), TEXT("string"));
	DestPathProp->SetStringField(TEXT("description"), TEXT("New asset path"));
	Properties->SetObjectField(TEXT("destination_path"), DestPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("source_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("destination_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FRenameAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FAssetRenameResult RenameResult = AssetModule.RenameAsset(SourcePath, DestinationPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (RenameResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Asset renamed successfully.\nNewPath: %s"),
			*RenameResult.NewPath);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to rename asset: %s"), *RenameResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
