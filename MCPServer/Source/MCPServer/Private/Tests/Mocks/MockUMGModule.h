// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IUMGModule.h"
#include "MockCallRecorder.h"

class FMockUMGModule : public IUMGModule
{
public:
	FMockCallRecorder Recorder;

	FCreateWidgetBlueprintResult CreateWidgetBlueprintResult;
	FGetWidgetTreeResult GetWidgetTreeResult;
	FAddWidgetResult AddWidgetResult;
	FRemoveWidgetResult RemoveWidgetResult;
	FMoveWidgetResult MoveWidgetResult;
	FRenameWidgetResult RenameWidgetResult;
	FReplaceWidgetResult ReplaceWidgetResult;
	FSetWidgetPropertyResult SetWidgetPropertyResult;
	FGetWidgetPropertyResult GetWidgetPropertyResult;
	FSetWidgetSlotResult SetWidgetSlotResult;
	FGetWidgetAnimationsResult GetWidgetAnimationsResult;
	FExportWidgetsResult ExportWidgetsResult;
	FImportWidgetsResult ImportWidgetsResult;

	virtual FCreateWidgetBlueprintResult CreateWidgetBlueprint(const FString& BlueprintPath, const FString* RootWidgetClass) override { Recorder.RecordCall(TEXT("CreateWidgetBlueprint")); return CreateWidgetBlueprintResult; }
	virtual FGetWidgetTreeResult GetWidgetTree(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("GetWidgetTree")); return GetWidgetTreeResult; }
	virtual FAddWidgetResult AddWidget(const FString& BlueprintPath, const FString& WidgetClass, const FString& ParentName, const FString* WidgetName, const int32* InsertIndex) override { Recorder.RecordCall(TEXT("AddWidget")); return AddWidgetResult; }
	virtual FRemoveWidgetResult RemoveWidget(const FString& BlueprintPath, const FString& WidgetName) override { Recorder.RecordCall(TEXT("RemoveWidget")); return RemoveWidgetResult; }
	virtual FMoveWidgetResult MoveWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewParentName, const int32* InsertIndex) override { Recorder.RecordCall(TEXT("MoveWidget")); return MoveWidgetResult; }
	virtual FRenameWidgetResult RenameWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewName) override { Recorder.RecordCall(TEXT("RenameWidget")); return RenameWidgetResult; }
	virtual FReplaceWidgetResult ReplaceWidget(const FString& BlueprintPath, const FString& WidgetName, const FString& NewWidgetClass) override { Recorder.RecordCall(TEXT("ReplaceWidget")); return ReplaceWidgetResult; }
	virtual FSetWidgetPropertyResult SetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName, const FString& PropertyValue) override { Recorder.RecordCall(TEXT("SetWidgetProperty")); return SetWidgetPropertyResult; }
	virtual FGetWidgetPropertyResult GetWidgetProperty(const FString& BlueprintPath, const FString& WidgetName, const FString& PropertyName) override { Recorder.RecordCall(TEXT("GetWidgetProperty")); return GetWidgetPropertyResult; }
	virtual FSetWidgetSlotResult SetWidgetSlot(const FString& BlueprintPath, const FString& WidgetName, const TMap<FString, FString>& SlotProperties) override { Recorder.RecordCall(TEXT("SetWidgetSlot")); return SetWidgetSlotResult; }
	virtual FGetWidgetAnimationsResult GetWidgetAnimations(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("GetWidgetAnimations")); return GetWidgetAnimationsResult; }
	virtual FExportWidgetsResult ExportWidgets(const FString& BlueprintPath, const TArray<FString>* WidgetNames) override { Recorder.RecordCall(TEXT("ExportWidgets")); return ExportWidgetsResult; }
	virtual FImportWidgetsResult ImportWidgets(const FString& BlueprintPath, const FString& ExportedText, const FString* ParentName) override { Recorder.RecordCall(TEXT("ImportWidgets")); return ImportWidgetsResult; }
};
