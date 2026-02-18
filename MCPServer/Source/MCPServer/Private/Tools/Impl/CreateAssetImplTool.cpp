// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateAssetImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FCreateAssetImplTool::FCreateAssetImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FCreateAssetImplTool::GetName() const
{
	return TEXT("create_asset");
}

FString FCreateAssetImplTool::GetDescription() const
{
	return TEXT("Create a new asset of the specified class.");
}

TSharedPtr<FJsonObject> FCreateAssetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetNameProp = MakeShared<FJsonObject>();
	AssetNameProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetNameProp->SetStringField(TEXT("description"), TEXT("Name of the asset to create"));
	Properties->SetObjectField(TEXT("asset_name"), AssetNameProp);

	TSharedPtr<FJsonObject> PackagePathProp = MakeShared<FJsonObject>();
	PackagePathProp->SetStringField(TEXT("type"), TEXT("string"));
	PackagePathProp->SetStringField(TEXT("description"),
		TEXT("Package path for the asset (e.g. '/Game/Path')"));
	Properties->SetObjectField(TEXT("package_path"), PackagePathProp);

	TSharedPtr<FJsonObject> AssetClassProp = MakeShared<FJsonObject>();
	AssetClassProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetClassProp->SetStringField(TEXT("description"), TEXT("Full class path of the asset type"));
	Properties->SetObjectField(TEXT("asset_class"), AssetClassProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("package_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetName;
	FString PackagePath;
	FString AssetClass;
	if (!Arguments.IsValid()
		|| !Arguments->TryGetStringField(TEXT("asset_name"), AssetName)
		|| !Arguments->TryGetStringField(TEXT("package_path"), PackagePath)
		|| !Arguments->TryGetStringField(TEXT("asset_class"), AssetClass))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: asset_name, package_path, and asset_class are all required"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetCreateResult CreateResult = AssetModule.CreateAsset(AssetName, PackagePath, AssetClass);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CreateResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Asset created successfully.\nAssetName: %s\nAssetPath: %s\nAssetClass: %s"),
			*CreateResult.AssetName, *CreateResult.AssetPath, *CreateResult.AssetClass);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create asset: %s"), *CreateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
