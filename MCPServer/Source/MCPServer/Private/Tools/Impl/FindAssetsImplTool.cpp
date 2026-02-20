// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/FindAssetsImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FFindAssetsImplTool::FFindAssetsImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FFindAssetsImplTool::GetName() const
{
	return TEXT("find_assets");
}

FString FFindAssetsImplTool::GetDescription() const
{
	return TEXT("Find assets by path, class, or tags.");
}

TSharedPtr<FJsonObject> FFindAssetsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PackagePathProp = MakeShared<FJsonObject>();
	PackagePathProp->SetStringField(TEXT("type"), TEXT("string"));
	PackagePathProp->SetStringField(TEXT("description"),
		TEXT("Package path to search in (e.g. '/Game/Meshes')"));
	Properties->SetObjectField(TEXT("package_path"), PackagePathProp);

	TSharedPtr<FJsonObject> ClassNameProp = MakeShared<FJsonObject>();
	ClassNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ClassNameProp->SetStringField(TEXT("description"),
		TEXT("Class name to filter by (e.g. 'StaticMesh')"));
	Properties->SetObjectField(TEXT("class_name"), ClassNameProp);

	TSharedPtr<FJsonObject> RecursiveProp = MakeShared<FJsonObject>();
	RecursiveProp->SetStringField(TEXT("type"), TEXT("boolean"));
	RecursiveProp->SetStringField(TEXT("description"),
		TEXT("Whether to search recursively (default: true)"));
	Properties->SetObjectField(TEXT("recursive"), RecursiveProp);

	TSharedPtr<FJsonObject> NameFilterProp = MakeShared<FJsonObject>();
	NameFilterProp->SetStringField(TEXT("type"), TEXT("string"));
	NameFilterProp->SetStringField(TEXT("description"),
		TEXT("Wildcard filter for asset name (e.g. '*Repair*', 'BP_Old_Bus_*')"));
	Properties->SetObjectField(TEXT("name_filter"), NameFilterProp);

	TSharedPtr<FJsonObject> LimitProp = MakeShared<FJsonObject>();
	LimitProp->SetStringField(TEXT("type"), TEXT("integer"));
	LimitProp->SetStringField(TEXT("description"),
		TEXT("Maximum number of results (default: 0 = no limit)"));
	Properties->SetObjectField(TEXT("limit"), LimitProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FFindAssetsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString PackagePath;
	FString ClassName;
	bool bRecursive = true;
	FString NameFilter;
	int32 Limit = 0;

	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("package_path"), PackagePath);
		Arguments->TryGetStringField(TEXT("class_name"), ClassName);
		Arguments->TryGetBoolField(TEXT("recursive"), bRecursive);
		Arguments->TryGetStringField(TEXT("name_filter"), NameFilter);
		double LimitD = 0;
		if (Arguments->TryGetNumberField(TEXT("limit"), LimitD))
		{
			Limit = static_cast<int32>(LimitD);
		}
	}

	FAssetFindResult FindResult = AssetModule.FindAssets(PackagePath, ClassName, bRecursive, NameFilter, Limit);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (FindResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Found %d assets:\n"), FindResult.Assets.Num());
		for (const FAssetInfo& Info : FindResult.Assets)
		{
			ResponseText += FString::Printf(
				TEXT("- AssetName: %s  AssetPath: %s  AssetClass: %s  PackagePath: %s\n"),
				*Info.AssetName, *Info.AssetPath, *Info.AssetClass, *Info.PackagePath);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to find assets: %s"), *FindResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
