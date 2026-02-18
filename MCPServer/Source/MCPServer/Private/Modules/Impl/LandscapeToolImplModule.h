// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ILandscapeToolModule.h"

class ALandscapeProxy;

class FLandscapeToolImplModule : public ILandscapeToolModule
{
public:
	virtual FLandscapeHeightmapResult GetLandscapeHeightmap(const FString& LandscapeActorName) override;
	virtual FLandscapeSetHeightmapResult SetLandscapeHeightmap(const FString& LandscapeActorName, const FString& FilePath) override;
	virtual FLandscapeWeightmapResult GetLandscapeWeightmap(const FString& LandscapeActorName) override;
	virtual FLandscapeSetWeightmapResult SetLandscapeWeightmap(const FString& LandscapeActorName, const FString& LayerName, const FString& FilePath) override;
	virtual FLandscapeImportResult ImportLandscape(const FString& HeightmapFilePath, const FVector& Location, const FVector& Scale) override;
	virtual FLandscapeExportResult ExportLandscape(const FString& LandscapeActorName, const FString& OutputFilePath) override;
	virtual FLandscapeRebuildResult RebuildLandscape(const FString& LandscapeActorName) override;

private:
	ALandscapeProxy* FindLandscapeByName(const FString& Name) const;
};
