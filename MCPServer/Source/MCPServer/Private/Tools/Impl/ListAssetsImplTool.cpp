// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ListAssetsImplTool.h"
#include "Modules/Interfaces/IAssetModule.h"
#include "Dom/JsonValue.h"

FListAssetsImplTool::FListAssetsImplTool(IAssetModule& InAssetModule)
	: AssetModule(InAssetModule)
{
}

FString FListAssetsImplTool::GetName() const
{
	return TEXT("list_assets");
}

FString FListAssetsImplTool::GetDescription() const
{
	return TEXT("List all assets in a directory.");
}

TSharedPtr<FJsonObject> FListAssetsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> DirectoryPathProp = MakeShared<FJsonObject>();
	DirectoryPathProp->SetStringField(TEXT("type"), TEXT("string"));
	DirectoryPathProp->SetStringField(TEXT("description"),
		TEXT("Directory path to list assets from (e.g. '/Game/Meshes')"));
	Properties->SetObjectField(TEXT("directory_path"), DirectoryPathProp);

	TSharedPtr<FJsonObject> RecursiveProp = MakeShared<FJsonObject>();
	RecursiveProp->SetStringField(TEXT("type"), TEXT("boolean"));
	RecursiveProp->SetStringField(TEXT("description"),
		TEXT("Whether to list recursively (default: true)"));
	Properties->SetObjectField(TEXT("recursive"), RecursiveProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("directory_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FListAssetsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString DirectoryPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("directory_path"), DirectoryPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: directory_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	bool bRecursive = true;
	if (Arguments.IsValid())
	{
		Arguments->TryGetBoolField(TEXT("recursive"), bRecursive);
	}

	FAssetListResult ListResult = AssetModule.ListAssets(DirectoryPath, bRecursive);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ListResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Found %d assets:\n"), ListResult.AssetPaths.Num());
		for (const FString& Path : ListResult.AssetPaths)
		{
			ResponseText += FString::Printf(TEXT("- %s\n"), *Path);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to list assets: %s"), *ListResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
