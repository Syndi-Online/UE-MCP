// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "MockCallRecorder.h"

class FMockLandscapeModule : public ILandscapeToolModule
{
public:
	FMockCallRecorder Recorder;

	FLandscapeHeightmapResult GetLandscapeHeightmapResult;
	FLandscapeSetHeightmapResult SetLandscapeHeightmapResult;
	FLandscapeWeightmapResult GetLandscapeWeightmapResult;
	FLandscapeSetWeightmapResult SetLandscapeWeightmapResult;
	FLandscapeImportResult ImportLandscapeResult;
	FLandscapeExportResult ExportLandscapeResult;
	FLandscapeRebuildResult RebuildLandscapeResult;

	virtual FLandscapeHeightmapResult GetLandscapeHeightmap(const FString& LandscapeActorName) override { Recorder.RecordCall(TEXT("GetLandscapeHeightmap")); return GetLandscapeHeightmapResult; }
	virtual FLandscapeSetHeightmapResult SetLandscapeHeightmap(const FString& LandscapeActorName, const FString& FilePath) override { Recorder.RecordCall(TEXT("SetLandscapeHeightmap")); return SetLandscapeHeightmapResult; }
	virtual FLandscapeWeightmapResult GetLandscapeWeightmap(const FString& LandscapeActorName) override { Recorder.RecordCall(TEXT("GetLandscapeWeightmap")); return GetLandscapeWeightmapResult; }
	virtual FLandscapeSetWeightmapResult SetLandscapeWeightmap(const FString& LandscapeActorName, const FString& LayerName, const FString& FilePath) override { Recorder.RecordCall(TEXT("SetLandscapeWeightmap")); return SetLandscapeWeightmapResult; }
	virtual FLandscapeImportResult ImportLandscape(const FString& HeightmapFilePath, const FVector& Location, const FVector& Scale) override { Recorder.RecordCall(TEXT("ImportLandscape")); return ImportLandscapeResult; }
	virtual FLandscapeExportResult ExportLandscape(const FString& LandscapeActorName, const FString& OutputFilePath) override { Recorder.RecordCall(TEXT("ExportLandscape")); return ExportLandscapeResult; }
	virtual FLandscapeRebuildResult RebuildLandscape(const FString& LandscapeActorName) override { Recorder.RecordCall(TEXT("RebuildLandscape")); return RebuildLandscapeResult; }
};
