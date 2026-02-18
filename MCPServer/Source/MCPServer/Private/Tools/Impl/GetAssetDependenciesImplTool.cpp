// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetAssetDependenciesImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FGetAssetDependenciesImplTool::FGetAssetDependenciesImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FGetAssetDependenciesImplTool::GetName() const
{
	return TEXT("get_asset_dependencies");
}

FString FGetAssetDependenciesImplTool::GetDescription() const
{
	return TEXT("Get the list of assets that a given asset depends on.");
}

TSharedPtr<FJsonObject> FGetAssetDependenciesImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path to query dependencies for"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetAssetDependenciesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FAssetDependenciesResult DepsResult = AssetModule.GetAssetDependencies(AssetPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DepsResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Asset '%s' has %d dependencies:\n"), *AssetPath, DepsResult.Dependencies.Num());
		for (const FString& Dep : DepsResult.Dependencies)
		{
			ResponseText += FString::Printf(TEXT("- %s\n"), *Dep);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get asset dependencies: %s"), *DepsResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
