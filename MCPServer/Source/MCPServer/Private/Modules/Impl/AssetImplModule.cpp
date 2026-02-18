// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/AssetImplModule.h"
#include "Editor.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AssetRegistry/AssetData.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "Factories/Factory.h"
#include "UObject/UObjectGlobals.h"

FAssetLoadResult FAssetImplModule::LoadAsset(const FString& AssetPath)
{
	FAssetLoadResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	UObject* Asset = AssetSubsystem->LoadAsset(AssetPath);
	if (!Asset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to load asset: %s"), *AssetPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.AssetName = Asset->GetName();
	Result.AssetPath = AssetPath;
	Result.AssetClass = Asset->GetClass()->GetPathName();
	return Result;
}

FAssetCreateResult FAssetImplModule::CreateAsset(const FString& AssetName, const FString& PackagePath, const FString& AssetClassName)
{
	FAssetCreateResult Result;

	UClass* AssetClass = FindObject<UClass>(nullptr, *AssetClassName);
	if (!AssetClass)
	{
		AssetClass = LoadClass<UObject>(nullptr, *AssetClassName);
	}
	if (!AssetClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Class not found: %s"), *AssetClassName);
		return Result;
	}

	// Find appropriate factory
	UFactory* Factory = nullptr;
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;
		if (Class->IsChildOf(UFactory::StaticClass()) && !Class->HasAnyClassFlags(CLASS_Abstract))
		{
			UFactory* DefaultFactory = Class->GetDefaultObject<UFactory>();
			if (DefaultFactory->CanCreateNew() && DefaultFactory->SupportedClass == AssetClass)
			{
				Factory = NewObject<UFactory>(GetTransientPackage(), Class);
				break;
			}
		}
	}

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();
	UObject* NewAsset = AssetTools.CreateAsset(AssetName, PackagePath, AssetClass, Factory);
	if (!NewAsset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create asset '%s' of class '%s' in '%s'"),
			*AssetName, *AssetClassName, *PackagePath);
		return Result;
	}

	Result.bSuccess = true;
	Result.AssetName = NewAsset->GetName();
	Result.AssetPath = NewAsset->GetPathName();
	Result.AssetClass = NewAsset->GetClass()->GetPathName();
	return Result;
}

FAssetDuplicateResult FAssetImplModule::DuplicateAsset(const FString& SourcePath, const FString& DestinationPath)
{
	FAssetDuplicateResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	UObject* DuplicatedAsset = AssetSubsystem->DuplicateAsset(SourcePath, DestinationPath);
	if (!DuplicatedAsset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to duplicate '%s' to '%s'"), *SourcePath, *DestinationPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.AssetName = DuplicatedAsset->GetName();
	Result.AssetPath = DestinationPath;
	return Result;
}

FAssetRenameResult FAssetImplModule::RenameAsset(const FString& SourcePath, const FString& DestinationPath)
{
	FAssetRenameResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	bool bRenamed = AssetSubsystem->RenameAsset(SourcePath, DestinationPath);
	if (!bRenamed)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to rename '%s' to '%s'"), *SourcePath, *DestinationPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.NewPath = DestinationPath;
	return Result;
}

FAssetDeleteResult FAssetImplModule::DeleteAsset(const FString& AssetPath)
{
	FAssetDeleteResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	bool bDeleted = AssetSubsystem->DeleteAsset(AssetPath);
	if (!bDeleted)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to delete asset: %s"), *AssetPath);
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

FAssetSaveResult FAssetImplModule::SaveAsset(const FString& AssetPath)
{
	FAssetSaveResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	bool bSaved = AssetSubsystem->SaveAsset(AssetPath, false);
	if (!bSaved)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to save asset: %s"), *AssetPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.AssetPath = AssetPath;
	return Result;
}

FAssetFindResult FAssetImplModule::FindAssets(const FString& PackagePath, const FString& ClassName, bool bRecursive)
{
	FAssetFindResult Result;

	IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();

	FARFilter Filter;
	if (!PackagePath.IsEmpty())
	{
		Filter.PackagePaths.Add(FName(*PackagePath));
		Filter.bRecursivePaths = bRecursive;
	}
	if (!ClassName.IsEmpty())
	{
		UClass* FilterClass = FindObject<UClass>(nullptr, *ClassName);
		if (!FilterClass)
		{
			FilterClass = LoadClass<UObject>(nullptr, *ClassName);
		}
		if (FilterClass)
		{
			Filter.ClassPaths.Add(FilterClass->GetClassPathName());
			Filter.bRecursiveClasses = true;
		}
	}

	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssets(Filter, AssetDataList);

	for (const FAssetData& Data : AssetDataList)
	{
		FAssetInfo Info;
		Info.AssetName = Data.AssetName.ToString();
		Info.AssetPath = Data.GetObjectPathString();
		Info.AssetClass = Data.AssetClassPath.ToString();
		Info.PackagePath = Data.PackagePath.ToString();
		Result.Assets.Add(Info);
	}

	Result.bSuccess = true;
	return Result;
}

FAssetListResult FAssetImplModule::ListAssets(const FString& DirectoryPath, bool bRecursive)
{
	FAssetListResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	Result.AssetPaths = AssetSubsystem->ListAssets(DirectoryPath, bRecursive, false);
	Result.bSuccess = true;
	return Result;
}

FAssetImportResult FAssetImplModule::ImportAsset(const FString& FilePath, const FString& DestinationPath)
{
	FAssetImportResult Result;

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();

	TArray<FString> Files;
	Files.Add(FilePath);

	TArray<UObject*> ImportedObjects = AssetTools.ImportAssets(Files, DestinationPath);

	if (ImportedObjects.Num() == 0)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to import '%s' to '%s'"), *FilePath, *DestinationPath);
		return Result;
	}

	for (UObject* Obj : ImportedObjects)
	{
		if (Obj)
		{
			Result.ImportedAssets.Add(Obj->GetPathName());
		}
	}

	Result.bSuccess = true;
	return Result;
}

FAssetExportResult FAssetImplModule::ExportAsset(const FString& AssetPath, const FString& ExportPath)
{
	FAssetExportResult Result;

	IAssetTools& AssetTools = FAssetToolsModule::GetModule().Get();

	TArray<FString> AssetsToExport;
	AssetsToExport.Add(AssetPath);

	AssetTools.ExportAssets(AssetsToExport, ExportPath);

	Result.bSuccess = true;
	return Result;
}

FAssetDependenciesResult FAssetImplModule::GetAssetDependencies(const FString& AssetPath)
{
	FAssetDependenciesResult Result;

	IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();

	// Convert asset path to package name
	FString PackageName = AssetPath;
	if (PackageName.Contains(TEXT(".")))
	{
		PackageName = FPackageName::ObjectPathToPackageName(AssetPath);
	}

	TArray<FName> Dependencies;
	AssetRegistry.GetDependencies(FName(*PackageName), Dependencies);

	for (const FName& Dep : Dependencies)
	{
		Result.Dependencies.Add(Dep.ToString());
	}

	Result.bSuccess = true;
	return Result;
}

FAssetReferencersResult FAssetImplModule::GetAssetReferencers(const FString& AssetPath)
{
	FAssetReferencersResult Result;

	IAssetRegistry& AssetRegistry = IAssetRegistry::GetChecked();

	FString PackageName = AssetPath;
	if (PackageName.Contains(TEXT(".")))
	{
		PackageName = FPackageName::ObjectPathToPackageName(AssetPath);
	}

	TArray<FName> Referencers;
	AssetRegistry.GetReferencers(FName(*PackageName), Referencers);

	for (const FName& Ref : Referencers)
	{
		Result.Referencers.Add(Ref.ToString());
	}

	Result.bSuccess = true;
	return Result;
}

FAssetMetadataResult FAssetImplModule::GetAssetMetadata(const FString& AssetPath)
{
	FAssetMetadataResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	UObject* Asset = AssetSubsystem->LoadAsset(AssetPath);
	if (!Asset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Asset not found: %s"), *AssetPath);
		return Result;
	}

	TMap<FName, FString> MetaTags = AssetSubsystem->GetMetadataTagValues(Asset);
	for (const auto& Pair : MetaTags)
	{
		Result.Tags.Add(Pair.Key.ToString(), Pair.Value);
	}

	Result.bSuccess = true;
	return Result;
}

FAssetSetMetadataResult FAssetImplModule::SetAssetMetadata(const FString& AssetPath, const FString& TagName, const FString& TagValue)
{
	FAssetSetMetadataResult Result;

	UEditorAssetSubsystem* AssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (!AssetSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorAssetSubsystem is not available");
		return Result;
	}

	UObject* Asset = AssetSubsystem->LoadAsset(AssetPath);
	if (!Asset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Asset not found: %s"), *AssetPath);
		return Result;
	}

	AssetSubsystem->SetMetadataTag(Asset, FName(*TagName), TagValue);

	Result.bSuccess = true;
	return Result;
}
