// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IUIModule.h"

class FUIImplModule : public IUIModule
{
public:
	virtual FNotificationResult ShowNotification(const FString& Message, const FString& SubText, float Duration) override;
	virtual FDialogResult ShowDialog(const FString& Title, const FString& Message, const FString& DialogType) override;
	virtual FAssetEditorResult OpenAssetEditor(const FString& AssetPath) override;
	virtual FAssetEditorResult CloseAssetEditor(const FString& AssetPath) override;
	virtual FNotificationResult SyncContentBrowser(const FString& Path) override;
	virtual FContentBrowserSelectionResult GetSelectedContentBrowser() override;
};
