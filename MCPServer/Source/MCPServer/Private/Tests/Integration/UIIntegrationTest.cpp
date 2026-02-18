// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockUIModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/ShowNotificationImplTool.h"
#include "Tools/Impl/SyncContentBrowserImplTool.h"
#include "Tools/Impl/OpenAssetEditorImplTool.h"
#include "Tools/Impl/CloseAssetEditorImplTool.h"
#include "Tools/Impl/GetSelectedContentBrowserImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FUIIntegrationTest,
	"MCPServer.Integration.UI.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FUIIntegrationTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;

	// Step 1: Show notification
	Mock.ShowNotificationResult.bSuccess = true;
	FShowNotificationImplTool NotifyTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("message"), TEXT("Test notification"));
	TestTrue(TEXT("ShowNotification success"), MCPTestUtils::IsSuccess(NotifyTool.Execute(Args)));

	// Step 2: Sync content browser
	Mock.SyncContentBrowserResult.bSuccess = true;
	FSyncContentBrowserImplTool SyncTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("path"), TEXT("/Game/Materials"));
	TestTrue(TEXT("SyncContentBrowser success"), MCPTestUtils::IsSuccess(SyncTool.Execute(Args)));

	// Step 3: Open asset editor
	Mock.OpenAssetEditorResult.bSuccess = true;
	Mock.OpenAssetEditorResult.AssetPath = TEXT("/Game/TestAsset");
	FOpenAssetEditorImplTool OpenTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/TestAsset"));
	TestTrue(TEXT("OpenAssetEditor success"), MCPTestUtils::IsSuccess(OpenTool.Execute(Args)));

	// Step 4: Close asset editor
	Mock.CloseAssetEditorResult.bSuccess = true;
	FCloseAssetEditorImplTool CloseTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/TestAsset"));
	TestTrue(TEXT("CloseAssetEditor success"), MCPTestUtils::IsSuccess(CloseTool.Execute(Args)));

	// Step 5: Get selected content browser
	Mock.GetSelectedContentBrowserResult.bSuccess = true;
	Mock.GetSelectedContentBrowserResult.CurrentPath = TEXT("/Game");
	FGetSelectedContentBrowserImplTool GetSelTool(Mock);
	TestTrue(TEXT("GetSelected success"), MCPTestUtils::IsSuccess(GetSelTool.Execute(MakeShared<FJsonObject>())));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 5);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
