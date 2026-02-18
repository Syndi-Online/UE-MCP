// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IUIModule.h"
#include "MockCallRecorder.h"

class FMockUIModule : public IUIModule
{
public:
	FMockCallRecorder Recorder;

	FNotificationResult ShowNotificationResult;
	FDialogResult ShowDialogResult;
	FAssetEditorResult OpenAssetEditorResult;
	FAssetEditorResult CloseAssetEditorResult;
	FNotificationResult SyncContentBrowserResult;
	FContentBrowserSelectionResult GetSelectedContentBrowserResult;

	virtual FNotificationResult ShowNotification(const FString& Message, const FString& SubText, float Duration) override { Recorder.RecordCall(TEXT("ShowNotification")); return ShowNotificationResult; }
	virtual FDialogResult ShowDialog(const FString& Title, const FString& Message, const FString& DialogType) override { Recorder.RecordCall(TEXT("ShowDialog")); return ShowDialogResult; }
	virtual FAssetEditorResult OpenAssetEditor(const FString& AssetPath) override { Recorder.RecordCall(TEXT("OpenAssetEditor")); return OpenAssetEditorResult; }
	virtual FAssetEditorResult CloseAssetEditor(const FString& AssetPath) override { Recorder.RecordCall(TEXT("CloseAssetEditor")); return CloseAssetEditorResult; }
	virtual FNotificationResult SyncContentBrowser(const FString& Path) override { Recorder.RecordCall(TEXT("SyncContentBrowser")); return SyncContentBrowserResult; }
	virtual FContentBrowserSelectionResult GetSelectedContentBrowser() override { Recorder.RecordCall(TEXT("GetSelectedContentBrowser")); return GetSelectedContentBrowserResult; }
};
