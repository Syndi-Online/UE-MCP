// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/LandscapeToolImplModule.h"
#include "Landscape.h"
#include "LandscapeProxy.h"
#include "LandscapeInfo.h"
#include "LandscapeEdit.h"
#include "LandscapeDataAccess.h"
#include "LandscapeSubsystem.h"
#include "LandscapeEditTypes.h"
#include "LandscapeLayerInfoObject.h"
#include "LandscapeEditorUtils.h"
#include "LandscapeImportHelper.h"
#include "EngineUtils.h"
#include "Editor.h"

ALandscapeProxy* FLandscapeToolImplModule::FindLandscapeByName(const FString& Name) const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		return nullptr;
	}

	for (TActorIterator<ALandscapeProxy> It(World); It; ++It)
	{
		if (It->GetActorLabel().Equals(Name, ESearchCase::IgnoreCase) ||
			It->GetName().Equals(Name, ESearchCase::IgnoreCase))
		{
			return *It;
		}
	}

	// If no name specified, return first landscape found
	if (Name.IsEmpty())
	{
		for (TActorIterator<ALandscapeProxy> It(World); It; ++It)
		{
			return *It;
		}
	}

	return nullptr;
}

FLandscapeHeightmapResult FLandscapeToolImplModule::GetLandscapeHeightmap(const FString& LandscapeActorName)
{
	FLandscapeHeightmapResult Result;

	ALandscapeProxy* Landscape = FindLandscapeByName(LandscapeActorName);
	if (!Landscape)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Landscape not found: %s"), *LandscapeActorName);
		return Result;
	}

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	if (!LandscapeInfo)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Landscape info not available");
		return Result;
	}

	LandscapeInfo->GetLandscapeExtent(Result.MinX, Result.MinY, Result.MaxX, Result.MaxY);
	Result.Width = Result.MaxX - Result.MinX + 1;
	Result.Height = Result.MaxY - Result.MinY + 1;
	Result.Scale = Landscape->GetActorScale3D();

	// Gather layer names
	for (const FLandscapeInfoLayerSettings& LayerSettings : LandscapeInfo->Layers)
	{
		if (LayerSettings.LayerInfoObj)
		{
			Result.LayerNames.Add(LayerSettings.LayerInfoObj->GetLayerName().ToString());
		}
	}

	Result.bSuccess = true;
	return Result;
}

FLandscapeSetHeightmapResult FLandscapeToolImplModule::SetLandscapeHeightmap(const FString& LandscapeActorName, const FString& FilePath)
{
	FLandscapeSetHeightmapResult Result;

	ALandscapeProxy* Landscape = FindLandscapeByName(LandscapeActorName);
	if (!Landscape)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Landscape not found: %s"), *LandscapeActorName);
		return Result;
	}

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	if (!LandscapeInfo)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Landscape info not available");
		return Result;
	}

	FLandscapeImportDescriptor ImportDescriptor;
	FText OutMessage;
	ELandscapeImportResult ImportResult = FLandscapeImportHelper::GetHeightmapImportDescriptor(
		FilePath, true, false, ImportDescriptor, OutMessage);

	if (ImportResult == ELandscapeImportResult::Error)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to read heightmap file: %s"), *OutMessage.ToString());
		return Result;
	}

	TArray<uint16> HeightData;
	ImportResult = FLandscapeImportHelper::GetHeightmapImportData(ImportDescriptor, 0, HeightData, OutMessage);

	if (ImportResult == ELandscapeImportResult::Error)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to import heightmap data: %s"), *OutMessage.ToString());
		return Result;
	}

	bool bSet = LandscapeEditorUtils::SetHeightmapData(Landscape, HeightData);
	if (!bSet)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to set heightmap data. Dimensions may not match the landscape.");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FLandscapeWeightmapResult FLandscapeToolImplModule::GetLandscapeWeightmap(const FString& LandscapeActorName)
{
	FLandscapeWeightmapResult Result;

	ALandscapeProxy* Landscape = FindLandscapeByName(LandscapeActorName);
	if (!Landscape)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Landscape not found: %s"), *LandscapeActorName);
		return Result;
	}

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	if (!LandscapeInfo)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Landscape info not available");
		return Result;
	}

	for (const FLandscapeInfoLayerSettings& LayerSettings : LandscapeInfo->Layers)
	{
		if (LayerSettings.LayerInfoObj)
		{
			Result.LayerNames.Add(LayerSettings.LayerInfoObj->GetLayerName().ToString());
		}
	}

	Result.bSuccess = true;
	return Result;
}

FLandscapeSetWeightmapResult FLandscapeToolImplModule::SetLandscapeWeightmap(const FString& LandscapeActorName, const FString& LayerName, const FString& FilePath)
{
	FLandscapeSetWeightmapResult Result;

	ALandscapeProxy* Landscape = FindLandscapeByName(LandscapeActorName);
	if (!Landscape)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Landscape not found: %s"), *LandscapeActorName);
		return Result;
	}

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	if (!LandscapeInfo)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Landscape info not available");
		return Result;
	}

	ULandscapeLayerInfoObject* LayerInfo = LandscapeInfo->GetLayerInfoByName(FName(*LayerName));
	if (!LayerInfo)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Layer not found: %s"), *LayerName);
		return Result;
	}

	FLandscapeImportDescriptor ImportDescriptor;
	FText OutMessage;
	ELandscapeImportResult ImportResult = FLandscapeImportHelper::GetWeightmapImportDescriptor(
		FilePath, true, false, FName(*LayerName), ImportDescriptor, OutMessage);

	if (ImportResult == ELandscapeImportResult::Error)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to read weightmap file: %s"), *OutMessage.ToString());
		return Result;
	}

	TArray<uint8> WeightData;
	ImportResult = FLandscapeImportHelper::GetWeightmapImportData(ImportDescriptor, 0, FName(*LayerName), WeightData, OutMessage);

	if (ImportResult == ELandscapeImportResult::Error)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to import weightmap data: %s"), *OutMessage.ToString());
		return Result;
	}

	bool bSet = LandscapeEditorUtils::SetWeightmapData(Landscape, LayerInfo, WeightData);
	if (!bSet)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to set weightmap data. Dimensions may not match the landscape.");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FLandscapeImportResult FLandscapeToolImplModule::ImportLandscape(const FString& HeightmapFilePath, const FVector& Location, const FVector& Scale)
{
	FLandscapeImportResult Result;

	FLandscapeImportDescriptor ImportDescriptor;
	FText OutMessage;
	ELandscapeImportResult ImportResult = FLandscapeImportHelper::GetHeightmapImportDescriptor(
		HeightmapFilePath, true, false, ImportDescriptor, OutMessage);

	if (ImportResult == ELandscapeImportResult::Error)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to read heightmap file: %s"), *OutMessage.ToString());
		return Result;
	}

	TArray<uint16> HeightData;
	ImportResult = FLandscapeImportHelper::GetHeightmapImportData(ImportDescriptor, 0, HeightData, OutMessage);

	if (ImportResult == ELandscapeImportResult::Error)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to import heightmap data: %s"), *OutMessage.ToString());
		return Result;
	}

	if (ImportDescriptor.ImportResolutions.Num() == 0)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No valid resolution found in heightmap file.");
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FLandscapeExportResult FLandscapeToolImplModule::ExportLandscape(const FString& LandscapeActorName, const FString& OutputFilePath)
{
	FLandscapeExportResult Result;

	ALandscapeProxy* Landscape = FindLandscapeByName(LandscapeActorName);
	if (!Landscape)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Landscape not found: %s"), *LandscapeActorName);
		return Result;
	}

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	if (!LandscapeInfo)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Landscape info not available");
		return Result;
	}

	LandscapeInfo->ExportHeightmap(OutputFilePath);

	Result.bSuccess = true;
	Result.FilePath = OutputFilePath;
	return Result;
}

FLandscapeRebuildResult FLandscapeToolImplModule::RebuildLandscape(const FString& LandscapeActorName)
{
	FLandscapeRebuildResult Result;

	ALandscapeProxy* Landscape = FindLandscapeByName(LandscapeActorName);
	if (!Landscape)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Landscape not found: %s"), *LandscapeActorName);
		return Result;
	}

	UWorld* World = Landscape->GetWorld();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("World not available");
		return Result;
	}

	ULandscapeSubsystem* Subsystem = World->GetSubsystem<ULandscapeSubsystem>();
	if (Subsystem)
	{
		Subsystem->BuildAll(UE::Landscape::EBuildFlags::None);
	}

	ALandscape* LandscapeActor = Cast<ALandscape>(Landscape);
	if (LandscapeActor)
	{
		LandscapeActor->ForceLayersFullUpdate();
	}

	ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();
	if (LandscapeInfo)
	{
		LandscapeInfo->RecreateCollisionComponents();
	}

	Result.bSuccess = true;
	return Result;
}
