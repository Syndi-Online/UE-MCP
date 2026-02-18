// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FLandscapeHeightmapResult
{
	bool bSuccess = false;
	int32 MinX = 0;
	int32 MinY = 0;
	int32 MaxX = 0;
	int32 MaxY = 0;
	int32 Width = 0;
	int32 Height = 0;
	FVector Scale = FVector::OneVector;
	TArray<FString> LayerNames;
	FString ErrorMessage;
};

struct FLandscapeSetHeightmapResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FLandscapeWeightmapResult
{
	bool bSuccess = false;
	TArray<FString> LayerNames;
	FString ErrorMessage;
};

struct FLandscapeSetWeightmapResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FLandscapeImportResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FLandscapeExportResult
{
	bool bSuccess = false;
	FString FilePath;
	FString ErrorMessage;
};

struct FLandscapeRebuildResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

/**
 * Module interface for Landscape operations.
 */
class ILandscapeToolModule
{
public:
	virtual ~ILandscapeToolModule() = default;

	virtual FLandscapeHeightmapResult GetLandscapeHeightmap(const FString& LandscapeActorName) = 0;
	virtual FLandscapeSetHeightmapResult SetLandscapeHeightmap(const FString& LandscapeActorName, const FString& FilePath) = 0;
	virtual FLandscapeWeightmapResult GetLandscapeWeightmap(const FString& LandscapeActorName) = 0;
	virtual FLandscapeSetWeightmapResult SetLandscapeWeightmap(const FString& LandscapeActorName, const FString& LayerName, const FString& FilePath) = 0;
	virtual FLandscapeImportResult ImportLandscape(const FString& HeightmapFilePath, const FVector& Location, const FVector& Scale) = 0;
	virtual FLandscapeExportResult ExportLandscape(const FString& LandscapeActorName, const FString& OutputFilePath) = 0;
	virtual FLandscapeRebuildResult RebuildLandscape(const FString& LandscapeActorName) = 0;
};
