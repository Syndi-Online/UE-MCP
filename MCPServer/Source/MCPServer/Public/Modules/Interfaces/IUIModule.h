// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FNotificationResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FDialogResult
{
	bool bSuccess = false;
	FString UserResponse;
	FString ErrorMessage;
};

struct FAssetEditorResult
{
	bool bSuccess = false;
	FString AssetPath;
	FString ErrorMessage;
};

struct FContentBrowserSelectionResult
{
	bool bSuccess = false;
	TArray<FString> SelectedAssets;
	TArray<FString> SelectedFolders;
	FString CurrentPath;
	FString ErrorMessage;
};

/**
 * Module interface for UI and Notification operations.
 */
class IUIModule
{
public:
	virtual ~IUIModule() = default;

	virtual FNotificationResult ShowNotification(const FString& Message, const FString& SubText, float Duration) = 0;
	virtual FDialogResult ShowDialog(const FString& Title, const FString& Message, const FString& DialogType) = 0;
	virtual FAssetEditorResult OpenAssetEditor(const FString& AssetPath) = 0;
	virtual FAssetEditorResult CloseAssetEditor(const FString& AssetPath) = 0;
	virtual FNotificationResult SyncContentBrowser(const FString& Path) = 0;
	virtual FContentBrowserSelectionResult GetSelectedContentBrowser() = 0;
};
