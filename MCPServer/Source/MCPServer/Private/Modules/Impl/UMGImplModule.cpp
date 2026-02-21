// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/UMGImplModule.h"

FCreateWidgetBlueprintResult FUMGImplModule::CreateWidgetBlueprint(const FString& BlueprintPath, const FString* RootWidgetClass)
{
	FCreateWidgetBlueprintResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FGetWidgetTreeResult FUMGImplModule::GetWidgetTree(const FString& BlueprintPath)
{
	FGetWidgetTreeResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FAddWidgetResult FUMGImplModule::AddWidget(const FString& BlueprintPath, const FString& WidgetClass, const FString& ParentName, const FString* WidgetName, const int32* InsertIndex)
{
	FAddWidgetResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FRemoveWidgetResult FUMGImplModule::RemoveWidget(const FString& BlueprintPath, const FString& WidgetName)
{
	FRemoveWidgetResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FMoveWidgetResult FUMGImplModule::MoveWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewParentName, const int32* InsertIndex)
{
	FMoveWidgetResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FRenameWidgetResult FUMGImplModule::RenameWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewName)
{
	FRenameWidgetResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FReplaceWidgetResult FUMGImplModule::ReplaceWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewWidgetClass)
{
	FReplaceWidgetResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FSetWidgetPropertyResult FUMGImplModule::SetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName, const FString& PropertyValue)
{
	FSetWidgetPropertyResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FGetWidgetPropertyResult FUMGImplModule::GetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName)
{
	FGetWidgetPropertyResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FSetWidgetSlotResult FUMGImplModule::SetWidgetSlot(const FString& BlueprintPath, const FString& WidgetName, const TMap<FString, FString>& SlotProperties)
{
	FSetWidgetSlotResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FGetWidgetAnimationsResult FUMGImplModule::GetWidgetAnimations(const FString& BlueprintPath)
{
	FGetWidgetAnimationsResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FExportWidgetsResult FUMGImplModule::ExportWidgets(const FString& BlueprintPath, const TArray<FString>* WidgetNames)
{
	FExportWidgetsResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}

FImportWidgetsResult FUMGImplModule::ImportWidgets(const FString& BlueprintPath, const FString& ExportedText, const FString* ParentName)
{
	FImportWidgetsResult Result;
	Result.bSuccess = false;
	Result.ErrorMessage = TEXT("Not yet implemented");
	return Result;
}
