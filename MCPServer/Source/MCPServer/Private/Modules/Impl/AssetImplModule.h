// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IAssetModule.h"

class FAssetImplModule : public IAssetModule
{
public:
	virtual FAssetLoadResult LoadAsset(const FString& AssetPath) override;
	virtual FAssetCreateResult CreateAsset(const FString& AssetName, const FString& PackagePath, const FString& AssetClass) override;
	virtual FAssetDuplicateResult DuplicateAsset(const FString& SourcePath, const FString& DestinationPath) override;
	virtual FAssetRenameResult RenameAsset(const FString& SourcePath, const FString& DestinationPath) override;
	virtual FAssetDeleteResult DeleteAsset(const FString& AssetPath) override;
	virtual FAssetSaveResult SaveAsset(const FString& AssetPath) override;
	virtual FAssetFindResult FindAssets(const FString& PackagePath, const FString& ClassName, bool bRecursive, const FString& NameFilter = TEXT(""), int32 Limit = 0) override;
	virtual FAssetListResult ListAssets(const FString& DirectoryPath, bool bRecursive) override;
	virtual FAssetImportResult ImportAsset(const FString& FilePath, const FString& DestinationPath) override;
	virtual FAssetExportResult ExportAsset(const FString& AssetPath, const FString& ExportPath) override;
	virtual FAssetDependenciesResult GetAssetDependencies(const FString& AssetPath) override;
	virtual FAssetReferencersResult GetAssetReferencers(const FString& AssetPath) override;
	virtual FAssetMetadataResult GetAssetMetadata(const FString& AssetPath) override;
	virtual FAssetSetMetadataResult SetAssetMetadata(const FString& AssetPath, const FString& TagName, const FString& TagValue) override;

	virtual FAssetSetPropertyResult SetAssetProperty(const FString& AssetPath, const FString& PropertyName, const FString& PropertyValue) override;
	virtual FAssetGetPropertyResult GetAssetProperty(const FString& AssetPath, const FString& PropertyName) override;
	virtual FAssetFindReferencersOfClassResult FindReferencersOfClass(const FString& ClassPath, const FString& PackagePath, bool bRecursive) override;
};
