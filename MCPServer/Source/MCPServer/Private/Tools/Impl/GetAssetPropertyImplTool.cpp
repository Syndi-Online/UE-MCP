// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetAssetPropertyImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FGetAssetPropertyImplTool::FGetAssetPropertyImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FGetAssetPropertyImplTool::GetName() const
{
	return TEXT("get_asset_property");
}

FString FGetAssetPropertyImplTool::GetDescription() const
{
	return TEXT("Read the value of a UPROPERTY on any asset");
}

TSharedPtr<FJsonObject> FGetAssetPropertyImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	TSharedPtr<FJsonObject> PropertyNameProp = MakeShared<FJsonObject>();
	PropertyNameProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyNameProp->SetStringField(TEXT("description"), TEXT("UPROPERTY name"));
	Properties->SetObjectField(TEXT("property_name"), PropertyNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetAssetPropertyImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetPath, PropertyName;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("asset_path"), AssetPath) ||
		!Arguments->TryGetStringField(TEXT("property_name"), PropertyName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: asset_path, property_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetGetPropertyResult GetResult = AssetModule.GetAssetProperty(AssetPath, PropertyName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (GetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("%s = %s"), *PropertyName, *GetResult.PropertyValue));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get property: %s"), *GetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
