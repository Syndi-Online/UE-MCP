// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/UIImplModule.h"
#include "Editor.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Misc/MessageDialog.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "AssetRegistry/AssetData.h"

FNotificationResult FUIImplModule::ShowNotification(const FString& Message, const FString& SubText, float Duration)
{
	FNotificationResult Result;

	FNotificationInfo Info(FText::FromString(Message));
	if (!SubText.IsEmpty())
	{
		Info.SubText = FText::FromString(SubText);
	}
	Info.bFireAndForget = true;
	Info.ExpireDuration = Duration;
	Info.bUseThrobber = false;
	Info.bUseSuccessFailIcons = false;

	TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
	if (Notification.IsValid())
	{
		Result.bSuccess = true;
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to create notification");
	}
	return Result;
}

FDialogResult FUIImplModule::ShowDialog(const FString& Title, const FString& Message, const FString& DialogType)
{
	FDialogResult Result;

	EAppMsgType::Type MsgType = EAppMsgType::Ok;
	if (DialogType.Equals(TEXT("YesNo"), ESearchCase::IgnoreCase))
	{
		MsgType = EAppMsgType::YesNo;
	}
	else if (DialogType.Equals(TEXT("OkCancel"), ESearchCase::IgnoreCase))
	{
		MsgType = EAppMsgType::OkCancel;
	}
	else if (DialogType.Equals(TEXT("YesNoCancel"), ESearchCase::IgnoreCase))
	{
		MsgType = EAppMsgType::YesNoCancel;
	}

	EAppReturnType::Type ReturnType = FMessageDialog::Open(
		MsgType,
		FText::FromString(Message),
		FText::FromString(Title)
	);

	Result.bSuccess = true;
	switch (ReturnType)
	{
	case EAppReturnType::Yes:    Result.UserResponse = TEXT("Yes"); break;
	case EAppReturnType::No:     Result.UserResponse = TEXT("No"); break;
	case EAppReturnType::Ok:     Result.UserResponse = TEXT("Ok"); break;
	case EAppReturnType::Cancel: Result.UserResponse = TEXT("Cancel"); break;
	case EAppReturnType::Retry:  Result.UserResponse = TEXT("Retry"); break;
	default:                     Result.UserResponse = TEXT("Unknown"); break;
	}
	return Result;
}

FAssetEditorResult FUIImplModule::OpenAssetEditor(const FString& AssetPath)
{
	FAssetEditorResult Result;

	if (!GEditor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Editor not available");
		return Result;
	}

	UAssetEditorSubsystem* AES = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (!AES)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Asset editor subsystem not available");
		return Result;
	}

	UObject* Asset = LoadObject<UObject>(nullptr, *AssetPath);
	if (!Asset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to load asset: %s"), *AssetPath);
		return Result;
	}

	bool bOpened = AES->OpenEditorForAsset(Asset);
	Result.bSuccess = bOpened;
	Result.AssetPath = AssetPath;
	if (!bOpened)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Failed to open editor for: %s"), *AssetPath);
	}
	return Result;
}

FAssetEditorResult FUIImplModule::CloseAssetEditor(const FString& AssetPath)
{
	FAssetEditorResult Result;

	if (!GEditor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Editor not available");
		return Result;
	}

	UAssetEditorSubsystem* AES = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (!AES)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Asset editor subsystem not available");
		return Result;
	}

	UObject* Asset = FindObject<UObject>(nullptr, *AssetPath);
	if (!Asset)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Asset not found in memory: %s"), *AssetPath);
		return Result;
	}

	int32 NumClosed = AES->CloseAllEditorsForAsset(Asset);
	Result.bSuccess = true;
	Result.AssetPath = AssetPath;
	return Result;
}

FNotificationResult FUIImplModule::SyncContentBrowser(const FString& Path)
{
	FNotificationResult Result;

	if (Path.Contains(TEXT(".")))
	{
		// Looks like an asset path — sync to asset
		FAssetData AssetData;
		AssetData.PackageName = FName(*Path);
		IContentBrowserSingleton::Get().SyncBrowserToAssets(TArray<FAssetData>{ AssetData }, false, true);
	}
	else
	{
		// Folder path
		IContentBrowserSingleton::Get().SyncBrowserToFolders(TArray<FString>{ Path }, false, true);
	}

	Result.bSuccess = true;
	return Result;
}

FContentBrowserSelectionResult FUIImplModule::GetSelectedContentBrowser()
{
	FContentBrowserSelectionResult Result;

	TArray<FAssetData> SelectedAssets;
	IContentBrowserSingleton::Get().GetSelectedAssets(SelectedAssets);
	for (const FAssetData& Asset : SelectedAssets)
	{
		Result.SelectedAssets.Add(Asset.GetObjectPathString());
	}

	IContentBrowserSingleton::Get().GetSelectedFolders(Result.SelectedFolders);

	Result.bSuccess = true;
	return Result;
}
