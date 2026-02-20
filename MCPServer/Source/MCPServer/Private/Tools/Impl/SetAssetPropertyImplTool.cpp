// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetAssetPropertyImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FSetAssetPropertyImplTool::FSetAssetPropertyImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FSetAssetPropertyImplTool::GetName() const
{
	return TEXT("set_asset_property");
}

FString FSetAssetPropertyImplTool::GetDescription() const
{
	return TEXT("Set the value of a UPROPERTY on any asset (SoundWave, Material, DataAsset, etc.)");
}

TSharedPtr<FJsonObject> FSetAssetPropertyImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path, e.g. /Game/Sounds/TireSkid_Ice"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	TSharedPtr<FJsonObject> PropertyNameProp = MakeShared<FJsonObject>();
	PropertyNameProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyNameProp->SetStringField(TEXT("description"), TEXT("UPROPERTY name, e.g. bLooping, VolumeMultiplier"));
	Properties->SetObjectField(TEXT("property_name"), PropertyNameProp);

	TSharedPtr<FJsonObject> PropertyValueProp = MakeShared<FJsonObject>();
	PropertyValueProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyValueProp->SetStringField(TEXT("description"), TEXT("Value in string format"));
	Properties->SetObjectField(TEXT("property_value"), PropertyValueProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_value")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetAssetPropertyImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetPath, PropertyName, PropertyValue;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("asset_path"), AssetPath) ||
		!Arguments->TryGetStringField(TEXT("property_name"), PropertyName) ||
		!Arguments->TryGetStringField(TEXT("property_value"), PropertyValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: asset_path, property_name, property_value"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetSetPropertyResult SetResult = AssetModule.SetAssetProperty(AssetPath, PropertyName, PropertyValue);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Property '%s' set on asset '%s'"), *PropertyName, *AssetPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set property: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
