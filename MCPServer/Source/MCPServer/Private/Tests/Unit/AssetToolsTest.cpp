// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/LoadAssetImplTool.h"
#include "Tools/Impl/CreateAssetImplTool.h"
#include "Tools/Impl/DuplicateAssetImplTool.h"
#include "Tools/Impl/RenameAssetImplTool.h"
#include "Tools/Impl/DeleteAssetImplTool.h"
#include "Tools/Impl/SaveAssetImplTool.h"
#include "Tools/Impl/FindAssetsImplTool.h"
#include "Tools/Impl/ListAssetsImplTool.h"
#include "Tools/Impl/ImportAssetImplTool.h"
#include "Tools/Impl/ExportAssetImplTool.h"
#include "Tools/Impl/GetAssetDependenciesImplTool.h"
#include "Tools/Impl/GetAssetReferencersImplTool.h"
#include "Tools/Impl/GetAssetMetadataImplTool.h"
#include "Tools/Impl/SetAssetMetadataImplTool.h"
#include "Tools/Impl/SetAssetPropertyImplTool.h"
#include "Tools/Impl/GetAssetPropertyImplTool.h"
#include "Tools/Impl/FindReferencersOfClassImplTool.h"
#include "Tests/Mocks/MockAssetModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// LoadAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadAssetMetadataTest,
	"MCPServer.Unit.Assets.LoadAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FLoadAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("load_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadAssetSuccessTest,
	"MCPServer.Unit.Assets.LoadAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.LoadAssetResult.bSuccess = true;
	Mock.LoadAssetResult.AssetName = TEXT("MyAsset");
	Mock.LoadAssetResult.AssetPath = TEXT("/Game/MyAsset");

	FLoadAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains asset name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("MyAsset")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadAssetMissingArgsTest,
	"MCPServer.Unit.Assets.LoadAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FLoadAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("LoadAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadAssetModuleFailureTest,
	"MCPServer.Unit.Assets.LoadAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.LoadAssetResult.bSuccess = false;
	Mock.LoadAssetResult.ErrorMessage = TEXT("Asset not found");

	FLoadAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Asset not found")));
	return true;
}

// ============================================================================
// CreateAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAssetMetadataTest,
	"MCPServer.Unit.Assets.CreateAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FCreateAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAssetSuccessTest,
	"MCPServer.Unit.Assets.CreateAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.CreateAssetResult.bSuccess = true;
	Mock.CreateAssetResult.AssetName = TEXT("NewAsset");
	Mock.CreateAssetResult.AssetPath = TEXT("/Game/NewAsset");

	FCreateAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_name"), TEXT("NewAsset"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game"));
	Args->SetStringField(TEXT("asset_class"), TEXT("Material"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains asset name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("NewAsset")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAssetMissingArgsTest,
	"MCPServer.Unit.Assets.CreateAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FCreateAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("CreateAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAssetModuleFailureTest,
	"MCPServer.Unit.Assets.CreateAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.CreateAssetResult.bSuccess = false;
	Mock.CreateAssetResult.ErrorMessage = TEXT("Create asset failed");

	FCreateAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_name"), TEXT("NewAsset"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game"));
	Args->SetStringField(TEXT("asset_class"), TEXT("Material"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Create asset failed")));
	return true;
}

// ============================================================================
// DuplicateAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateAssetMetadataTest,
	"MCPServer.Unit.Assets.DuplicateAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FDuplicateAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("duplicate_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateAssetSuccessTest,
	"MCPServer.Unit.Assets.DuplicateAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.DuplicateAssetResult.bSuccess = true;
	Mock.DuplicateAssetResult.AssetName = TEXT("DupAsset");

	FDuplicateAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("source_path"), TEXT("/Game/Original"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/DupAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains asset name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("DupAsset")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateAssetMissingArgsTest,
	"MCPServer.Unit.Assets.DuplicateAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FDuplicateAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("DuplicateAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateAssetModuleFailureTest,
	"MCPServer.Unit.Assets.DuplicateAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.DuplicateAssetResult.bSuccess = false;
	Mock.DuplicateAssetResult.ErrorMessage = TEXT("Duplicate asset failed");

	FDuplicateAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("source_path"), TEXT("/Game/Original"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/DupAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Duplicate asset failed")));
	return true;
}

// ============================================================================
// RenameAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameAssetMetadataTest,
	"MCPServer.Unit.Assets.RenameAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FRenameAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("rename_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameAssetSuccessTest,
	"MCPServer.Unit.Assets.RenameAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.RenameAssetResult.bSuccess = true;
	Mock.RenameAssetResult.NewPath = TEXT("/Game/Renamed");

	FRenameAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("source_path"), TEXT("/Game/Original"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/Renamed"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameAssetMissingArgsTest,
	"MCPServer.Unit.Assets.RenameAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FRenameAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("RenameAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenameAssetModuleFailureTest,
	"MCPServer.Unit.Assets.RenameAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRenameAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.RenameAssetResult.bSuccess = false;
	Mock.RenameAssetResult.ErrorMessage = TEXT("Rename failed");

	FRenameAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("source_path"), TEXT("/Game/Original"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/Renamed"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Rename failed")));
	return true;
}

// ============================================================================
// DeleteAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteAssetMetadataTest,
	"MCPServer.Unit.Assets.DeleteAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FDeleteAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("delete_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteAssetSuccessTest,
	"MCPServer.Unit.Assets.DeleteAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.DeleteAssetResult.bSuccess = true;

	FDeleteAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/ToDelete"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteAssetMissingArgsTest,
	"MCPServer.Unit.Assets.DeleteAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FDeleteAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("DeleteAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteAssetModuleFailureTest,
	"MCPServer.Unit.Assets.DeleteAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.DeleteAssetResult.bSuccess = false;
	Mock.DeleteAssetResult.ErrorMessage = TEXT("Delete failed");

	FDeleteAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/ToDelete"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Delete failed")));
	return true;
}

// ============================================================================
// SaveAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAssetMetadataTest,
	"MCPServer.Unit.Assets.SaveAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FSaveAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("save_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAssetSuccessTest,
	"MCPServer.Unit.Assets.SaveAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.SaveAssetResult.bSuccess = true;
	Mock.SaveAssetResult.AssetPath = TEXT("/Game/Saved");

	FSaveAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Saved"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAssetMissingArgsTest,
	"MCPServer.Unit.Assets.SaveAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FSaveAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SaveAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAssetModuleFailureTest,
	"MCPServer.Unit.Assets.SaveAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.SaveAssetResult.bSuccess = false;
	Mock.SaveAssetResult.ErrorMessage = TEXT("Save asset failed");

	FSaveAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Saved"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Save asset failed")));
	return true;
}

// ============================================================================
// FindAssets
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindAssetsMetadataTest,
	"MCPServer.Unit.Assets.FindAssets.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindAssetsMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FFindAssetsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("find_assets"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindAssetsSuccessTest,
	"MCPServer.Unit.Assets.FindAssets.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindAssetsSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.FindAssetsResult.bSuccess = true;
	FAssetInfo Info;
	Info.AssetName = TEXT("FoundAsset");
	Info.AssetPath = TEXT("/Game/FoundAsset");
	Info.AssetClass = TEXT("StaticMesh");
	Mock.FindAssetsResult.Assets.Add(Info);

	FFindAssetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("package_path"), TEXT("/Game"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains asset name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("FoundAsset")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindAssetsMissingArgsTest,
	"MCPServer.Unit.Assets.FindAssets.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindAssetsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.FindAssetsResult.bSuccess = true;

	FFindAssetsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// FindAssets has all optional args, so empty args still calls the module
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Mock called"), Mock.Recorder.GetCallCount(TEXT("FindAssets")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindAssetsModuleFailureTest,
	"MCPServer.Unit.Assets.FindAssets.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindAssetsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.FindAssetsResult.bSuccess = false;
	Mock.FindAssetsResult.ErrorMessage = TEXT("Find failed");

	FFindAssetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("package_path"), TEXT("/Game"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Find failed")));
	return true;
}

// ============================================================================
// ListAssets
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FListAssetsMetadataTest,
	"MCPServer.Unit.Assets.ListAssets.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FListAssetsMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FListAssetsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("list_assets"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FListAssetsSuccessTest,
	"MCPServer.Unit.Assets.ListAssets.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FListAssetsSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.ListAssetsResult.bSuccess = true;
	Mock.ListAssetsResult.AssetPaths.Add(TEXT("/Game/Asset1"));
	Mock.ListAssetsResult.AssetPaths.Add(TEXT("/Game/Asset2"));

	FListAssetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("directory_path"), TEXT("/Game"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Asset1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FListAssetsMissingArgsTest,
	"MCPServer.Unit.Assets.ListAssets.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FListAssetsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FListAssetsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ListAssets")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FListAssetsModuleFailureTest,
	"MCPServer.Unit.Assets.ListAssets.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FListAssetsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.ListAssetsResult.bSuccess = false;
	Mock.ListAssetsResult.ErrorMessage = TEXT("List failed");

	FListAssetsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("directory_path"), TEXT("/Game"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("List failed")));
	return true;
}

// ============================================================================
// ImportAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportAssetMetadataTest,
	"MCPServer.Unit.Assets.ImportAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FImportAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("import_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportAssetSuccessTest,
	"MCPServer.Unit.Assets.ImportAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.ImportAssetResult.bSuccess = true;
	Mock.ImportAssetResult.ImportedAssets.Add(TEXT("/Game/Imported"));

	FImportAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("file_path"), TEXT("C:/Assets/model.fbx"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/Imported"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportAssetMissingArgsTest,
	"MCPServer.Unit.Assets.ImportAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FImportAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ImportAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportAssetModuleFailureTest,
	"MCPServer.Unit.Assets.ImportAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.ImportAssetResult.bSuccess = false;
	Mock.ImportAssetResult.ErrorMessage = TEXT("Import failed");

	FImportAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("file_path"), TEXT("C:/Assets/model.fbx"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/Imported"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Import failed")));
	return true;
}

// ============================================================================
// ExportAsset
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportAssetMetadataTest,
	"MCPServer.Unit.Assets.ExportAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FExportAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("export_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportAssetSuccessTest,
	"MCPServer.Unit.Assets.ExportAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.ExportAssetResult.bSuccess = true;

	FExportAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyMesh"));
	Args->SetStringField(TEXT("export_path"), TEXT("C:/Export/mesh.fbx"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportAssetMissingArgsTest,
	"MCPServer.Unit.Assets.ExportAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FExportAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ExportAsset")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportAssetModuleFailureTest,
	"MCPServer.Unit.Assets.ExportAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.ExportAssetResult.bSuccess = false;
	Mock.ExportAssetResult.ErrorMessage = TEXT("Export failed");

	FExportAssetImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyMesh"));
	Args->SetStringField(TEXT("export_path"), TEXT("C:/Export/mesh.fbx"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Export failed")));
	return true;
}

// ============================================================================
// GetAssetDependencies
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetDependenciesMetadataTest,
	"MCPServer.Unit.Assets.GetAssetDependencies.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetDependenciesMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetDependenciesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_asset_dependencies"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetDependenciesSuccessTest,
	"MCPServer.Unit.Assets.GetAssetDependencies.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetDependenciesSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetDependenciesResult.bSuccess = true;
	Mock.GetAssetDependenciesResult.Dependencies.Add(TEXT("/Game/Dep1"));

	FGetAssetDependenciesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains dep"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Dep1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetDependenciesMissingArgsTest,
	"MCPServer.Unit.Assets.GetAssetDependencies.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetDependenciesMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetDependenciesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetAssetDependencies")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetDependenciesModuleFailureTest,
	"MCPServer.Unit.Assets.GetAssetDependencies.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetDependenciesModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetDependenciesResult.bSuccess = false;
	Mock.GetAssetDependenciesResult.ErrorMessage = TEXT("Dependencies failed");

	FGetAssetDependenciesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Dependencies failed")));
	return true;
}

// ============================================================================
// GetAssetReferencers
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetReferencersMetadataTest,
	"MCPServer.Unit.Assets.GetAssetReferencers.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetReferencersMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetReferencersImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_asset_referencers"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetReferencersSuccessTest,
	"MCPServer.Unit.Assets.GetAssetReferencers.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetReferencersSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetReferencersResult.bSuccess = true;
	Mock.GetAssetReferencersResult.Referencers.Add(TEXT("/Game/Ref1"));

	FGetAssetReferencersImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains ref"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Ref1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetReferencersMissingArgsTest,
	"MCPServer.Unit.Assets.GetAssetReferencers.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetReferencersMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetReferencersImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetAssetReferencers")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetReferencersModuleFailureTest,
	"MCPServer.Unit.Assets.GetAssetReferencers.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetReferencersModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetReferencersResult.bSuccess = false;
	Mock.GetAssetReferencersResult.ErrorMessage = TEXT("Referencers failed");

	FGetAssetReferencersImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Referencers failed")));
	return true;
}

// ============================================================================
// GetAssetMetadata
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetMetadataMetadataTest,
	"MCPServer.Unit.Assets.GetAssetMetadata.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetMetadataMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetMetadataImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_asset_metadata"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetMetadataSuccessTest,
	"MCPServer.Unit.Assets.GetAssetMetadata.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetMetadataSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetMetadataResult.bSuccess = true;
	Mock.GetAssetMetadataResult.Tags.Add(TEXT("Author"), TEXT("Test"));

	FGetAssetMetadataImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetMetadataMissingArgsTest,
	"MCPServer.Unit.Assets.GetAssetMetadata.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetMetadataMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetMetadataImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetAssetMetadata")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetMetadataModuleFailureTest,
	"MCPServer.Unit.Assets.GetAssetMetadata.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetMetadataModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetMetadataResult.bSuccess = false;
	Mock.GetAssetMetadataResult.ErrorMessage = TEXT("Metadata failed");

	FGetAssetMetadataImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Metadata failed")));
	return true;
}

// ============================================================================
// SetAssetMetadata
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetMetadataMetadataTest,
	"MCPServer.Unit.Assets.SetAssetMetadata.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetMetadataMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FSetAssetMetadataImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_asset_metadata"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetMetadataSuccessTest,
	"MCPServer.Unit.Assets.SetAssetMetadata.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetMetadataSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.SetAssetMetadataResult.bSuccess = true;

	FSetAssetMetadataImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));
	Args->SetStringField(TEXT("tag_name"), TEXT("Author"));
	Args->SetStringField(TEXT("tag_value"), TEXT("TestUser"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetMetadataMissingArgsTest,
	"MCPServer.Unit.Assets.SetAssetMetadata.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetMetadataMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FSetAssetMetadataImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetAssetMetadata")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetMetadataModuleFailureTest,
	"MCPServer.Unit.Assets.SetAssetMetadata.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetMetadataModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.SetAssetMetadataResult.bSuccess = false;
	Mock.SetAssetMetadataResult.ErrorMessage = TEXT("Set metadata failed");

	FSetAssetMetadataImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/MyAsset"));
	Args->SetStringField(TEXT("tag_name"), TEXT("Author"));
	Args->SetStringField(TEXT("tag_value"), TEXT("TestUser"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Set metadata failed")));
	return true;
}

// ============================================================================
// SetAssetProperty
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetPropertyMetadataTest,
	"MCPServer.Unit.Assets.SetAssetProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FSetAssetPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_asset_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetPropertySuccessTest,
	"MCPServer.Unit.Assets.SetAssetProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.SetAssetPropertyResult.bSuccess = true;

	FSetAssetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Sounds/MySound"));
	Args->SetStringField(TEXT("property_name"), TEXT("bLooping"));
	Args->SetStringField(TEXT("property_value"), TEXT("true"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains property name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("bLooping")));
	TestEqual(TEXT("Mock called"), Mock.Recorder.GetCallCount(TEXT("SetAssetProperty")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetPropertyMissingArgsTest,
	"MCPServer.Unit.Assets.SetAssetProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FSetAssetPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetAssetProperty")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetAssetPropertyModuleFailureTest,
	"MCPServer.Unit.Assets.SetAssetProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetAssetPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.SetAssetPropertyResult.bSuccess = false;
	Mock.SetAssetPropertyResult.ErrorMessage = TEXT("Property not found");

	FSetAssetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Sounds/MySound"));
	Args->SetStringField(TEXT("property_name"), TEXT("BadProp"));
	Args->SetStringField(TEXT("property_value"), TEXT("true"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Property not found")));
	return true;
}

// ============================================================================
// GetAssetProperty
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetPropertyMetadataTest,
	"MCPServer.Unit.Assets.GetAssetProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_asset_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetPropertySuccessTest,
	"MCPServer.Unit.Assets.GetAssetProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetPropertyResult.bSuccess = true;
	Mock.GetAssetPropertyResult.PropertyValue = TEXT("1.5");

	FGetAssetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Sounds/MySound"));
	Args->SetStringField(TEXT("property_name"), TEXT("VolumeMultiplier"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains property name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("VolumeMultiplier")));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("1.5")));
	TestEqual(TEXT("Mock called"), Mock.Recorder.GetCallCount(TEXT("GetAssetProperty")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetPropertyMissingArgsTest,
	"MCPServer.Unit.Assets.GetAssetProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FGetAssetPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetAssetProperty")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetAssetPropertyModuleFailureTest,
	"MCPServer.Unit.Assets.GetAssetProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetAssetPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.GetAssetPropertyResult.bSuccess = false;
	Mock.GetAssetPropertyResult.ErrorMessage = TEXT("Asset not found");

	FGetAssetPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Missing"));
	Args->SetStringField(TEXT("property_name"), TEXT("SomeProp"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Asset not found")));
	return true;
}

// ============================================================================
// FindReferencersOfClass
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindReferencersOfClassMetadataTest,
	"MCPServer.Unit.Assets.FindReferencersOfClass.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindReferencersOfClassMetadataTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FFindReferencersOfClassImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("find_referencers_of_class"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindReferencersOfClassSuccessTest,
	"MCPServer.Unit.Assets.FindReferencersOfClass.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindReferencersOfClassSuccessTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.FindReferencersOfClassResult.bSuccess = true;
	FAssetInfo Info;
	Info.AssetName = TEXT("BP_RepairStation");
	Info.AssetPath = TEXT("/Game/BP_RepairStation");
	Info.AssetClass = TEXT("Blueprint");
	Mock.FindReferencersOfClassResult.Assets.Add(Info);

	FFindReferencersOfClassImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("class_path"), TEXT("/Script/MyGame.RepairActivity"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains asset"), MCPTestUtils::GetResultText(Result).Contains(TEXT("BP_RepairStation")));
	TestEqual(TEXT("Mock called"), Mock.Recorder.GetCallCount(TEXT("FindReferencersOfClass")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindReferencersOfClassMissingArgsTest,
	"MCPServer.Unit.Assets.FindReferencersOfClass.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindReferencersOfClassMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	FFindReferencersOfClassImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("FindReferencersOfClass")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindReferencersOfClassModuleFailureTest,
	"MCPServer.Unit.Assets.FindReferencersOfClass.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindReferencersOfClassModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;
	Mock.FindReferencersOfClassResult.bSuccess = false;
	Mock.FindReferencersOfClassResult.ErrorMessage = TEXT("Class not found");

	FFindReferencersOfClassImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("class_path"), TEXT("/Script/Missing.BadClass"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Class not found")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
