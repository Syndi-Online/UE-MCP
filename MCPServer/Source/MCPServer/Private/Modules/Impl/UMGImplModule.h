// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IUMGModule.h"

class FUMGImplModule : public IUMGModule
{
public:
	virtual FCreateWidgetBlueprintResult CreateWidgetBlueprint(const FString& BlueprintPath, const FString* RootWidgetClass) override;
	virtual FGetWidgetTreeResult GetWidgetTree(const FString& BlueprintPath) override;
	virtual FAddWidgetResult AddWidget(const FString& BlueprintPath, const FString& WidgetClass, const FString& ParentName, const FString* WidgetName, const int32* InsertIndex) override;
	virtual FRemoveWidgetResult RemoveWidget(const FString& BlueprintPath, const FString& WidgetName) override;
	virtual FMoveWidgetResult MoveWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewParentName, const int32* InsertIndex) override;
	virtual FRenameWidgetResult RenameWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewName) override;
	virtual FReplaceWidgetResult ReplaceWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewWidgetClass) override;
	virtual FSetWidgetPropertyResult SetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName, const FString& PropertyValue) override;
	virtual FGetWidgetPropertyResult GetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName) override;
	virtual FSetWidgetSlotResult SetWidgetSlot(const FString& BlueprintPath, const FString& WidgetName, const TMap<FString, FString>& SlotProperties) override;
	virtual FGetWidgetAnimationsResult GetWidgetAnimations(const FString& BlueprintPath) override;
	virtual FExportWidgetsResult ExportWidgets(const FString& BlueprintPath, const TArray<FString>* WidgetNames) override;
	virtual FImportWidgetsResult ImportWidgets(const FString& BlueprintPath, const FString& ExportedText, const FString* ParentName) override;
};
