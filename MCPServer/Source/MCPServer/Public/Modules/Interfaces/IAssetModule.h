// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FAssetLoadResult
{
	bool bSuccess = false;
	FString AssetName;
	FString AssetPath;
	FString AssetClass;
	FString ErrorMessage;
};

struct FAssetCreateResult
{
	bool bSuccess = false;
	FString AssetName;
	FString AssetPath;
	FString AssetClass;
	FString ErrorMessage;
};

struct FAssetDuplicateResult
{
	bool bSuccess = false;
	FString AssetName;
	FString AssetPath;
	FString ErrorMessage;
};

struct FAssetRenameResult
{
	bool bSuccess = false;
	FString NewPath;
	FString ErrorMessage;
};

struct FAssetDeleteResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FAssetSaveResult
{
	bool bSuccess = false;
	FString AssetPath;
	FString ErrorMessage;
};

struct FAssetInfo
{
	FString AssetName;
	FString AssetPath;
	FString AssetClass;
	FString PackagePath;
};

struct FAssetFindResult
{
	bool bSuccess = false;
	TArray<FAssetInfo> Assets;
	FString ErrorMessage;
};

struct FAssetListResult
{
	bool bSuccess = false;
	TArray<FString> AssetPaths;
	FString ErrorMessage;
};

struct FAssetImportResult
{
	bool bSuccess = false;
	TArray<FString> ImportedAssets;
	FString ErrorMessage;
};

struct FAssetExportResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FAssetDependenciesResult
{
	bool bSuccess = false;
	TArray<FString> Dependencies;
	FString ErrorMessage;
};

struct FAssetReferencersResult
{
	bool bSuccess = false;
	TArray<FString> Referencers;
	FString ErrorMessage;
};

struct FAssetMetadataResult
{
	bool bSuccess = false;
	TMap<FString, FString> Tags;
	FString ErrorMessage;
};

struct FAssetSetMetadataResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FAssetSetPropertyResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FAssetGetPropertyResult
{
	bool bSuccess = false;
	FString PropertyValue;
	FString ErrorMessage;
};

struct FAssetFindReferencersOfClassResult
{
	bool bSuccess = false;
	TArray<FAssetInfo> Assets;
	FString ErrorMessage;
};

/**
 * Module interface for asset management operations.
 */
class IAssetModule
{
public:
	virtual ~IAssetModule() = default;

	virtual FAssetLoadResult LoadAsset(const FString& AssetPath) = 0;
	virtual FAssetCreateResult CreateAsset(const FString& AssetName, const FString& PackagePath, const FString& AssetClass) = 0;
	virtual FAssetDuplicateResult DuplicateAsset(const FString& SourcePath, const FString& DestinationPath) = 0;
	virtual FAssetRenameResult RenameAsset(const FString& SourcePath, const FString& DestinationPath) = 0;
	virtual FAssetDeleteResult DeleteAsset(const FString& AssetPath) = 0;
	virtual FAssetSaveResult SaveAsset(const FString& AssetPath) = 0;
	virtual FAssetFindResult FindAssets(const FString& PackagePath, const FString& ClassName, bool bRecursive, const FString& NameFilter = TEXT(""), int32 Limit = 0) = 0;
	virtual FAssetListResult ListAssets(const FString& DirectoryPath, bool bRecursive) = 0;
	virtual FAssetImportResult ImportAsset(const FString& FilePath, const FString& DestinationPath) = 0;
	virtual FAssetExportResult ExportAsset(const FString& AssetPath, const FString& ExportPath) = 0;
	virtual FAssetDependenciesResult GetAssetDependencies(const FString& AssetPath) = 0;
	virtual FAssetReferencersResult GetAssetReferencers(const FString& AssetPath) = 0;
	virtual FAssetMetadataResult GetAssetMetadata(const FString& AssetPath) = 0;
	virtual FAssetSetMetadataResult SetAssetMetadata(const FString& AssetPath, const FString& TagName, const FString& TagValue) = 0;

	virtual FAssetSetPropertyResult SetAssetProperty(const FString& AssetPath, const FString& PropertyName, const FString& PropertyValue) = 0;
	virtual FAssetGetPropertyResult GetAssetProperty(const FString& AssetPath, const FString& PropertyName) = 0;
	virtual FAssetFindReferencersOfClassResult FindReferencersOfClass(const FString& ClassPath, const FString& PackagePath, bool bRecursive) = 0;
};
