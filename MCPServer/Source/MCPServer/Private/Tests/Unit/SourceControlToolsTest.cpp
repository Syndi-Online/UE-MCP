// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/SccCheckoutImplTool.h"
#include "Tools/Impl/SccAddImplTool.h"
#include "Tools/Impl/SccDeleteImplTool.h"
#include "Tools/Impl/SccRevertImplTool.h"
#include "Tools/Impl/SccStatusImplTool.h"
#include "Tools/Impl/SccSubmitImplTool.h"
#include "Tools/Impl/SccSyncImplTool.h"
#include "Tests/Mocks/MockSCCModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// SCCCheckout
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCCheckoutMetadataTest,
	"MCPServer.Unit.SourceControl.SCCCheckout.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCCheckoutMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccCheckoutImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_checkout"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCCheckoutSuccessTest,
	"MCPServer.Unit.SourceControl.SCCCheckout.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCCheckoutSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.CheckoutResult.bSuccess = true;
	Mock.CheckoutResult.AffectedFiles.Add(TEXT("/Game/MyFile.uasset"));
	Mock.CheckoutResult.Message = TEXT("Checked out");

	FSccCheckoutImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Checkout")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCCheckoutMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCCheckout.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCCheckoutMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccCheckoutImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Checkout")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCCheckoutModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCCheckout.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCCheckoutModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.CheckoutResult.bSuccess = false;
	Mock.CheckoutResult.ErrorMessage = TEXT("File not in depot");

	FSccCheckoutImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/Missing.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Checkout")), 1);
	return true;
}

// ============================================================================
// SCCAdd
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCAddMetadataTest,
	"MCPServer.Unit.SourceControl.SCCAdd.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCAddMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccAddImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_add"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCAddSuccessTest,
	"MCPServer.Unit.SourceControl.SCCAdd.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCAddSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.AddResult.bSuccess = true;

	FSccAddImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/NewFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Add")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCAddMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCAdd.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCAddMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccAddImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Add")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCAddModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCAdd.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCAddModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.AddResult.bSuccess = false;
	Mock.AddResult.ErrorMessage = TEXT("Already in source control");

	FSccAddImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/Existing.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Add")), 1);
	return true;
}

// ============================================================================
// SCCDelete
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCDeleteMetadataTest,
	"MCPServer.Unit.SourceControl.SCCDelete.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCDeleteMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccDeleteImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_delete"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCDeleteSuccessTest,
	"MCPServer.Unit.SourceControl.SCCDelete.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCDeleteSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.DeleteResult.bSuccess = true;

	FSccDeleteImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/OldFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Delete")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCDeleteMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCDelete.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCDeleteMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccDeleteImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Delete")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCDeleteModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCDelete.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCDeleteModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.DeleteResult.bSuccess = false;
	Mock.DeleteResult.ErrorMessage = TEXT("File checked out by another user");

	FSccDeleteImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/Locked.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Delete")), 1);
	return true;
}

// ============================================================================
// SCCRevert
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCRevertMetadataTest,
	"MCPServer.Unit.SourceControl.SCCRevert.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCRevertMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccRevertImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_revert"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCRevertSuccessTest,
	"MCPServer.Unit.SourceControl.SCCRevert.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCRevertSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.RevertResult.bSuccess = true;

	FSccRevertImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Revert")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCRevertMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCRevert.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCRevertMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccRevertImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Revert")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCRevertModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCRevert.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCRevertModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.RevertResult.bSuccess = false;
	Mock.RevertResult.ErrorMessage = TEXT("Revert failed");

	FSccRevertImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Revert")), 1);
	return true;
}

// ============================================================================
// SCCStatus
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCStatusMetadataTest,
	"MCPServer.Unit.SourceControl.SCCStatus.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCStatusMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccStatusImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_status"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCStatusSuccessTest,
	"MCPServer.Unit.SourceControl.SCCStatus.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCStatusSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.StatusResult.bSuccess = true;
	FSCCStatusResult::FFileStatus FileStatus;
	FileStatus.FilePath = TEXT("/Game/MyFile.uasset");
	FileStatus.bIsSourceControlled = true;
	FileStatus.bIsCheckedOut = true;
	Mock.StatusResult.FileStatuses.Add(FileStatus);

	FSccStatusImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Status")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCStatusMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCStatus.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCStatusMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccStatusImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Status")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCStatusModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCStatus.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCStatusModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.StatusResult.bSuccess = false;
	Mock.StatusResult.ErrorMessage = TEXT("Not connected to source control");

	FSccStatusImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Status")), 1);
	return true;
}

// ============================================================================
// SCCSubmit
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSubmitMetadataTest,
	"MCPServer.Unit.SourceControl.SCCSubmit.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSubmitMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccSubmitImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_submit"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSubmitSuccessTest,
	"MCPServer.Unit.SourceControl.SCCSubmit.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSubmitSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.SubmitResult.bSuccess = true;

	FSccSubmitImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);
	Args->SetStringField(TEXT("description"), TEXT("Test submit"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Submit")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSubmitMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCSubmit.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSubmitMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccSubmitImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Submit")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSubmitModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCSubmit.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSubmitModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.SubmitResult.bSuccess = false;
	Mock.SubmitResult.ErrorMessage = TEXT("Submit failed");

	FSccSubmitImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);
	Args->SetStringField(TEXT("description"), TEXT("Test submit"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Submit")), 1);
	return true;
}

// ============================================================================
// SCCSync
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSyncMetadataTest,
	"MCPServer.Unit.SourceControl.SCCSync.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSyncMetadataTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccSyncImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("scc_sync"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSyncSuccessTest,
	"MCPServer.Unit.SourceControl.SCCSync.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSyncSuccessTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.SyncResult.bSuccess = true;

	FSccSyncImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Sync")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSyncMissingArgsTest,
	"MCPServer.Unit.SourceControl.SCCSync.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSyncMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	FSccSyncImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("Sync")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSCCSyncModuleFailureTest,
	"MCPServer.Unit.SourceControl.SCCSync.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSCCSyncModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;
	Mock.SyncResult.bSuccess = false;
	Mock.SyncResult.ErrorMessage = TEXT("Connection lost");

	FSccSyncImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/MyFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("Sync")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
