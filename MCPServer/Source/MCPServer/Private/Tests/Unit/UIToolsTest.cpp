// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/ShowNotificationImplTool.h"
#include "Tools/Impl/ShowDialogImplTool.h"
#include "Tools/Impl/OpenAssetEditorImplTool.h"
#include "Tools/Impl/CloseAssetEditorImplTool.h"
#include "Tools/Impl/SyncContentBrowserImplTool.h"
#include "Tools/Impl/GetSelectedContentBrowserImplTool.h"
#include "Tests/Mocks/MockUIModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// ShowNotification
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowNotificationMetadataTest,
	"MCPServer.Unit.UI.ShowNotification.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowNotificationMetadataTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FShowNotificationImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("show_notification"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowNotificationSuccessTest,
	"MCPServer.Unit.UI.ShowNotification.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowNotificationSuccessTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.ShowNotificationResult.bSuccess = true;

	FShowNotificationImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("message"), TEXT("Hello World"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ShowNotification")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowNotificationMissingArgsTest,
	"MCPServer.Unit.UI.ShowNotification.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowNotificationMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FShowNotificationImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("ShowNotification")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowNotificationModuleFailureTest,
	"MCPServer.Unit.UI.ShowNotification.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowNotificationModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.ShowNotificationResult.bSuccess = false;
	Mock.ShowNotificationResult.ErrorMessage = TEXT("Failed to show notification");

	FShowNotificationImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("message"), TEXT("Hello"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ShowNotification")), 1);
	return true;
}

// ============================================================================
// ShowDialog
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowDialogMetadataTest,
	"MCPServer.Unit.UI.ShowDialog.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowDialogMetadataTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FShowDialogImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("show_dialog"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowDialogSuccessTest,
	"MCPServer.Unit.UI.ShowDialog.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowDialogSuccessTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.ShowDialogResult.bSuccess = true;
	Mock.ShowDialogResult.UserResponse = TEXT("OK");

	FShowDialogImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("title"), TEXT("Confirm"));
	Args->SetStringField(TEXT("message"), TEXT("Are you sure?"));
	Args->SetStringField(TEXT("dialog_type"), TEXT("ok_cancel"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains response"), MCPTestUtils::GetResultText(Result).Contains(TEXT("OK")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ShowDialog")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowDialogMissingArgsTest,
	"MCPServer.Unit.UI.ShowDialog.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowDialogMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FShowDialogImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("ShowDialog")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShowDialogModuleFailureTest,
	"MCPServer.Unit.UI.ShowDialog.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FShowDialogModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.ShowDialogResult.bSuccess = false;
	Mock.ShowDialogResult.ErrorMessage = TEXT("Dialog failed");

	FShowDialogImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("title"), TEXT("Confirm"));
	Args->SetStringField(TEXT("message"), TEXT("Test"));
	Args->SetStringField(TEXT("dialog_type"), TEXT("ok"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ShowDialog")), 1);
	return true;
}

// ============================================================================
// OpenAssetEditor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenAssetEditorMetadataTest,
	"MCPServer.Unit.UI.OpenAssetEditor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenAssetEditorMetadataTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FOpenAssetEditorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("open_asset_editor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenAssetEditorSuccessTest,
	"MCPServer.Unit.UI.OpenAssetEditor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenAssetEditorSuccessTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.OpenAssetEditorResult.bSuccess = true;
	Mock.OpenAssetEditorResult.AssetPath = TEXT("/Game/Test");

	FOpenAssetEditorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Test"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains asset path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("/Game/Test")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("OpenAssetEditor")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenAssetEditorMissingArgsTest,
	"MCPServer.Unit.UI.OpenAssetEditor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenAssetEditorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FOpenAssetEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("OpenAssetEditor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOpenAssetEditorModuleFailureTest,
	"MCPServer.Unit.UI.OpenAssetEditor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FOpenAssetEditorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.OpenAssetEditorResult.bSuccess = false;
	Mock.OpenAssetEditorResult.ErrorMessage = TEXT("Asset not found");

	FOpenAssetEditorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Missing"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("OpenAssetEditor")), 1);
	return true;
}

// ============================================================================
// CloseAssetEditor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCloseAssetEditorMetadataTest,
	"MCPServer.Unit.UI.CloseAssetEditor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCloseAssetEditorMetadataTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FCloseAssetEditorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("close_asset_editor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCloseAssetEditorSuccessTest,
	"MCPServer.Unit.UI.CloseAssetEditor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCloseAssetEditorSuccessTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.CloseAssetEditorResult.bSuccess = true;

	FCloseAssetEditorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Test"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("CloseAssetEditor")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCloseAssetEditorMissingArgsTest,
	"MCPServer.Unit.UI.CloseAssetEditor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCloseAssetEditorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FCloseAssetEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("CloseAssetEditor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCloseAssetEditorModuleFailureTest,
	"MCPServer.Unit.UI.CloseAssetEditor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCloseAssetEditorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.CloseAssetEditorResult.bSuccess = false;
	Mock.CloseAssetEditorResult.ErrorMessage = TEXT("No editor open for asset");

	FCloseAssetEditorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/NotOpen"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("CloseAssetEditor")), 1);
	return true;
}

// ============================================================================
// SyncContentBrowser
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSyncContentBrowserMetadataTest,
	"MCPServer.Unit.UI.SyncContentBrowser.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSyncContentBrowserMetadataTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FSyncContentBrowserImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sync_content_browser"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSyncContentBrowserSuccessTest,
	"MCPServer.Unit.UI.SyncContentBrowser.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSyncContentBrowserSuccessTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.SyncContentBrowserResult.bSuccess = true;

	FSyncContentBrowserImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("path"), TEXT("/Game/Maps"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SyncContentBrowser")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSyncContentBrowserMissingArgsTest,
	"MCPServer.Unit.UI.SyncContentBrowser.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSyncContentBrowserMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FSyncContentBrowserImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("SyncContentBrowser")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSyncContentBrowserModuleFailureTest,
	"MCPServer.Unit.UI.SyncContentBrowser.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSyncContentBrowserModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.SyncContentBrowserResult.bSuccess = false;
	Mock.SyncContentBrowserResult.ErrorMessage = TEXT("Path not found");

	FSyncContentBrowserImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("path"), TEXT("/Game/Missing"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SyncContentBrowser")), 1);
	return true;
}

// ============================================================================
// GetSelectedContentBrowser
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSelectedContentBrowserMetadataTest,
	"MCPServer.Unit.UI.GetSelectedContentBrowser.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSelectedContentBrowserMetadataTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	FGetSelectedContentBrowserImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_selected_content_browser"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSelectedContentBrowserSuccessTest,
	"MCPServer.Unit.UI.GetSelectedContentBrowser.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSelectedContentBrowserSuccessTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.GetSelectedContentBrowserResult.bSuccess = true;
	Mock.GetSelectedContentBrowserResult.CurrentPath = TEXT("/Game");

	FGetSelectedContentBrowserImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetSelectedContentBrowser")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSelectedContentBrowserEmptyArgsTest,
	"MCPServer.Unit.UI.GetSelectedContentBrowser.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSelectedContentBrowserEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.GetSelectedContentBrowserResult.bSuccess = true;

	FGetSelectedContentBrowserImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetSelectedContentBrowser")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSelectedContentBrowserModuleFailureTest,
	"MCPServer.Unit.UI.GetSelectedContentBrowser.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSelectedContentBrowserModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockUIModule Mock;
	Mock.GetSelectedContentBrowserResult.bSuccess = false;
	Mock.GetSelectedContentBrowserResult.ErrorMessage = TEXT("No content browser open");

	FGetSelectedContentBrowserImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetSelectedContentBrowser")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
