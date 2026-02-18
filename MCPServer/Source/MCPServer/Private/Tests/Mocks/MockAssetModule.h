// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IAssetModule.h"
#include "MockCallRecorder.h"

class FMockAssetModule : public IAssetModule
{
public:
	FMockCallRecorder Recorder;

	FAssetLoadResult LoadAssetResult;
	FAssetCreateResult CreateAssetResult;
	FAssetDuplicateResult DuplicateAssetResult;
	FAssetRenameResult RenameAssetResult;
	FAssetDeleteResult DeleteAssetResult;
	FAssetSaveResult SaveAssetResult;
	FAssetFindResult FindAssetsResult;
	FAssetListResult ListAssetsResult;
	FAssetImportResult ImportAssetResult;
	FAssetExportResult ExportAssetResult;
	FAssetDependenciesResult GetAssetDependenciesResult;
	FAssetReferencersResult GetAssetReferencersResult;
	FAssetMetadataResult GetAssetMetadataResult;
	FAssetSetMetadataResult SetAssetMetadataResult;

	virtual FAssetLoadResult LoadAsset(const FString& AssetPath) override
	{
		Recorder.RecordCall(TEXT("LoadAsset"));
		return LoadAssetResult;
	}

	virtual FAssetCreateResult CreateAsset(const FString& AssetName, const FString& PackagePath, const FString& AssetClass) override
	{
		Recorder.RecordCall(TEXT("CreateAsset"));
		return CreateAssetResult;
	}

	virtual FAssetDuplicateResult DuplicateAsset(const FString& SourcePath, const FString& DestinationPath) override
	{
		Recorder.RecordCall(TEXT("DuplicateAsset"));
		return DuplicateAssetResult;
	}

	virtual FAssetRenameResult RenameAsset(const FString& SourcePath, const FString& DestinationPath) override
	{
		Recorder.RecordCall(TEXT("RenameAsset"));
		return RenameAssetResult;
	}

	virtual FAssetDeleteResult DeleteAsset(const FString& AssetPath) override
	{
		Recorder.RecordCall(TEXT("DeleteAsset"));
		return DeleteAssetResult;
	}

	virtual FAssetSaveResult SaveAsset(const FString& AssetPath) override
	{
		Recorder.RecordCall(TEXT("SaveAsset"));
		return SaveAssetResult;
	}

	virtual FAssetFindResult FindAssets(const FString& PackagePath, const FString& ClassName, bool bRecursive) override
	{
		Recorder.RecordCall(TEXT("FindAssets"));
		return FindAssetsResult;
	}

	virtual FAssetListResult ListAssets(const FString& DirectoryPath, bool bRecursive) override
	{
		Recorder.RecordCall(TEXT("ListAssets"));
		return ListAssetsResult;
	}

	virtual FAssetImportResult ImportAsset(const FString& FilePath, const FString& DestinationPath) override
	{
		Recorder.RecordCall(TEXT("ImportAsset"));
		return ImportAssetResult;
	}

	virtual FAssetExportResult ExportAsset(const FString& AssetPath, const FString& ExportPath) override
	{
		Recorder.RecordCall(TEXT("ExportAsset"));
		return ExportAssetResult;
	}

	virtual FAssetDependenciesResult GetAssetDependencies(const FString& AssetPath) override
	{
		Recorder.RecordCall(TEXT("GetAssetDependencies"));
		return GetAssetDependenciesResult;
	}

	virtual FAssetReferencersResult GetAssetReferencers(const FString& AssetPath) override
	{
		Recorder.RecordCall(TEXT("GetAssetReferencers"));
		return GetAssetReferencersResult;
	}

	virtual FAssetMetadataResult GetAssetMetadata(const FString& AssetPath) override
	{
		Recorder.RecordCall(TEXT("GetAssetMetadata"));
		return GetAssetMetadataResult;
	}

	virtual FAssetSetMetadataResult SetAssetMetadata(const FString& AssetPath, const FString& TagName, const FString& TagValue) override
	{
		Recorder.RecordCall(TEXT("SetAssetMetadata"));
		return SetAssetMetadataResult;
	}
};
