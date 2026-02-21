// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

// Helper structs

struct FWidgetInfo
{
	FString WidgetName;
	FString WidgetClass;
	FString ParentName;
	FString SlotClass;
	int32 ChildIndex = -1;
	TArray<FWidgetInfo> Children;
};

struct FWidgetAnimationInfo
{
	FString AnimationName;
	float StartTime = 0.0f;
	float EndTime = 0.0f;
};

// Result structs

struct FCreateWidgetBlueprintResult
{
	bool bSuccess = false;
	FString BlueprintName;
	FString BlueprintPath;
	FString RootWidgetClass;
	FString ErrorMessage;
};

struct FGetWidgetTreeResult
{
	bool bSuccess = false;
	FString RootWidgetName;
	FString RootWidgetClass;
	TArray<FWidgetInfo> Widgets;
	FString ErrorMessage;
};

struct FAddWidgetResult
{
	bool bSuccess = false;
	FString WidgetName;
	FString WidgetClass;
	FString ParentName;
	FString ErrorMessage;
};

struct FRemoveWidgetResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FMoveWidgetResult
{
	bool bSuccess = false;
	FString WidgetName;
	FString NewParentName;
	FString ErrorMessage;
};

struct FRenameWidgetResult
{
	bool bSuccess = false;
	FString OldName;
	FString NewName;
	FString ErrorMessage;
};

struct FReplaceWidgetResult
{
	bool bSuccess = false;
	FString WidgetName;
	FString OldClass;
	FString NewClass;
	FString ErrorMessage;
};

struct FSetWidgetPropertyResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FGetWidgetPropertyResult
{
	bool bSuccess = false;
	FString PropertyValue;
	FString ErrorMessage;
};

struct FSetWidgetSlotResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FGetWidgetAnimationsResult
{
	bool bSuccess = false;
	TArray<FWidgetAnimationInfo> Animations;
	FString ErrorMessage;
};

struct FExportWidgetsResult
{
	bool bSuccess = false;
	FString ExportedText;
	FString ErrorMessage;
};

struct FImportWidgetsResult
{
	bool bSuccess = false;
	int32 WidgetsImported = 0;
	FString ErrorMessage;
};

/**
 * Module interface for UMG Widget Blueprint operations.
 */
class IUMGModule
{
public:
	virtual ~IUMGModule() = default;

	virtual FCreateWidgetBlueprintResult CreateWidgetBlueprint(const FString& BlueprintPath, const FString* RootWidgetClass) = 0;
	virtual FGetWidgetTreeResult GetWidgetTree(const FString& BlueprintPath) = 0;
	virtual FAddWidgetResult AddWidget(const FString& BlueprintPath, const FString& WidgetClass, const FString& ParentName, const FString* WidgetName, const int32* InsertIndex) = 0;
	virtual FRemoveWidgetResult RemoveWidget(const FString& BlueprintPath, const FString& WidgetName) = 0;
	virtual FMoveWidgetResult MoveWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewParentName, const int32* InsertIndex) = 0;
	virtual FRenameWidgetResult RenameWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewName) = 0;
	virtual FReplaceWidgetResult ReplaceWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewWidgetClass) = 0;
	virtual FSetWidgetPropertyResult SetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName, const FString& PropertyValue) = 0;
	virtual FGetWidgetPropertyResult GetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName) = 0;
	virtual FSetWidgetSlotResult SetWidgetSlot(const FString& BlueprintPath, const FString& WidgetName, const TMap<FString, FString>& SlotProperties) = 0;
	virtual FGetWidgetAnimationsResult GetWidgetAnimations(const FString& BlueprintPath) = 0;
	virtual FExportWidgetsResult ExportWidgets(const FString& BlueprintPath, const TArray<FString>* WidgetNames) = 0;
	virtual FImportWidgetsResult ImportWidgets(const FString& BlueprintPath, const FString& ExportedText, const FString* ParentName) = 0;
};
