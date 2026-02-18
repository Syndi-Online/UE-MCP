// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/GetLandscapeHeightmapImplTool.h"
#include "Tools/Impl/SetLandscapeHeightmapImplTool.h"
#include "Tools/Impl/GetLandscapeWeightmapImplTool.h"
#include "Tools/Impl/SetLandscapeWeightmapImplTool.h"
#include "Tools/Impl/ImportLandscapeImplTool.h"
#include "Tools/Impl/ExportLandscapeImplTool.h"
#include "Tools/Impl/RebuildLandscapeImplTool.h"
#include "Tests/Mocks/MockLandscapeModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ---------------------------------------------------------------------------
// GetLandscapeHeightmap
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLandscapeHeightmapMetadataTest,
	"MCPServer.Unit.Landscape.GetLandscapeHeightmap.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLandscapeHeightmapMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FGetLandscapeHeightmapImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_landscape_heightmap"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLandscapeHeightmapSuccessTest,
	"MCPServer.Unit.Landscape.GetLandscapeHeightmap.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLandscapeHeightmapSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.GetLandscapeHeightmapResult.bSuccess = true;
	Mock.GetLandscapeHeightmapResult.Width = 1024;
	Mock.GetLandscapeHeightmapResult.Height = 1024;
	FGetLandscapeHeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Landscape_0"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 1024"), MCPTestUtils::GetResultText(Result).Contains(TEXT("1024")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLandscapeHeightmapModuleFailureTest,
	"MCPServer.Unit.Landscape.GetLandscapeHeightmap.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLandscapeHeightmapModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.GetLandscapeHeightmapResult.bSuccess = false;
	Mock.GetLandscapeHeightmapResult.ErrorMessage = TEXT("Landscape not found");
	FGetLandscapeHeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Landscape not found")));
	return true;
}

// ---------------------------------------------------------------------------
// SetLandscapeHeightmap
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeHeightmapMetadataTest,
	"MCPServer.Unit.Landscape.SetLandscapeHeightmap.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeHeightmapMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FSetLandscapeHeightmapImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_landscape_heightmap"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeHeightmapSuccessTest,
	"MCPServer.Unit.Landscape.SetLandscapeHeightmap.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeHeightmapSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.SetLandscapeHeightmapResult.bSuccess = true;
	FSetLandscapeHeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Landscape_0"));
	Args->SetStringField(TEXT("file_path"), TEXT("D:/heightmap.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains imported"), MCPTestUtils::GetResultText(Result).Contains(TEXT("imported")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeHeightmapMissingArgsTest,
	"MCPServer.Unit.Landscape.SetLandscapeHeightmap.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeHeightmapMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FSetLandscapeHeightmapImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions file_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("file_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeHeightmapModuleFailureTest,
	"MCPServer.Unit.Landscape.SetLandscapeHeightmap.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeHeightmapModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.SetLandscapeHeightmapResult.bSuccess = false;
	Mock.SetLandscapeHeightmapResult.ErrorMessage = TEXT("File not found");
	FSetLandscapeHeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("file_path"), TEXT("D:/missing.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("File not found")));
	return true;
}

// ---------------------------------------------------------------------------
// GetLandscapeWeightmap
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLandscapeWeightmapMetadataTest,
	"MCPServer.Unit.Landscape.GetLandscapeWeightmap.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLandscapeWeightmapMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FGetLandscapeWeightmapImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_landscape_weightmap"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLandscapeWeightmapSuccessTest,
	"MCPServer.Unit.Landscape.GetLandscapeWeightmap.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLandscapeWeightmapSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.GetLandscapeWeightmapResult.bSuccess = true;
	Mock.GetLandscapeWeightmapResult.LayerNames.Add(TEXT("Grass"));
	Mock.GetLandscapeWeightmapResult.LayerNames.Add(TEXT("Dirt"));
	FGetLandscapeWeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Landscape_0"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Grass"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Grass")));
	TestTrue(TEXT("Contains Dirt"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Dirt")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLandscapeWeightmapModuleFailureTest,
	"MCPServer.Unit.Landscape.GetLandscapeWeightmap.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLandscapeWeightmapModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.GetLandscapeWeightmapResult.bSuccess = false;
	Mock.GetLandscapeWeightmapResult.ErrorMessage = TEXT("Landscape not found");
	FGetLandscapeWeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Landscape not found")));
	return true;
}

// ---------------------------------------------------------------------------
// SetLandscapeWeightmap
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeWeightmapMetadataTest,
	"MCPServer.Unit.Landscape.SetLandscapeWeightmap.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeWeightmapMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FSetLandscapeWeightmapImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_landscape_weightmap"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeWeightmapSuccessTest,
	"MCPServer.Unit.Landscape.SetLandscapeWeightmap.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeWeightmapSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.SetLandscapeWeightmapResult.bSuccess = true;
	FSetLandscapeWeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Landscape_0"));
	Args->SetStringField(TEXT("layer_name"), TEXT("Grass"));
	Args->SetStringField(TEXT("file_path"), TEXT("D:/grass_weight.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Grass"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Grass")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeWeightmapMissingArgsTest,
	"MCPServer.Unit.Landscape.SetLandscapeWeightmap.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeWeightmapMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FSetLandscapeWeightmapImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions layer_name"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("layer_name")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLandscapeWeightmapModuleFailureTest,
	"MCPServer.Unit.Landscape.SetLandscapeWeightmap.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLandscapeWeightmapModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.SetLandscapeWeightmapResult.bSuccess = false;
	Mock.SetLandscapeWeightmapResult.ErrorMessage = TEXT("Layer not found");
	FSetLandscapeWeightmapImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("layer_name"), TEXT("BadLayer"));
	Args->SetStringField(TEXT("file_path"), TEXT("D:/weight.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Layer not found")));
	return true;
}

// ---------------------------------------------------------------------------
// ImportLandscape
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLandscapeMetadataTest,
	"MCPServer.Unit.Landscape.ImportLandscape.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLandscapeMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FImportLandscapeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("import_landscape"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLandscapeSuccessTest,
	"MCPServer.Unit.Landscape.ImportLandscape.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLandscapeSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.ImportLandscapeResult.bSuccess = true;
	FImportLandscapeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("heightmap_file_path"), TEXT("D:/heightmap.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains loaded"), MCPTestUtils::GetResultText(Result).Contains(TEXT("loaded")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLandscapeMissingArgsTest,
	"MCPServer.Unit.Landscape.ImportLandscape.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLandscapeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FImportLandscapeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions heightmap_file_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("heightmap_file_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLandscapeModuleFailureTest,
	"MCPServer.Unit.Landscape.ImportLandscape.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLandscapeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.ImportLandscapeResult.bSuccess = false;
	Mock.ImportLandscapeResult.ErrorMessage = TEXT("Invalid heightmap dimensions");
	FImportLandscapeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("heightmap_file_path"), TEXT("D:/bad.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid heightmap dimensions")));
	return true;
}

// ---------------------------------------------------------------------------
// ExportLandscape
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportLandscapeMetadataTest,
	"MCPServer.Unit.Landscape.ExportLandscape.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportLandscapeMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FExportLandscapeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("export_landscape"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportLandscapeSuccessTest,
	"MCPServer.Unit.Landscape.ExportLandscape.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportLandscapeSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.ExportLandscapeResult.bSuccess = true;
	Mock.ExportLandscapeResult.FilePath = TEXT("/tmp/hm.png");
	FExportLandscapeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Landscape_0"));
	Args->SetStringField(TEXT("output_file_path"), TEXT("/tmp/hm.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains /tmp/hm.png"), MCPTestUtils::GetResultText(Result).Contains(TEXT("/tmp/hm.png")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportLandscapeMissingArgsTest,
	"MCPServer.Unit.Landscape.ExportLandscape.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportLandscapeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FExportLandscapeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions output_file_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("output_file_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExportLandscapeModuleFailureTest,
	"MCPServer.Unit.Landscape.ExportLandscape.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExportLandscapeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.ExportLandscapeResult.bSuccess = false;
	Mock.ExportLandscapeResult.ErrorMessage = TEXT("Cannot write to path");
	FExportLandscapeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("output_file_path"), TEXT("/readonly/hm.png"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Cannot write to path")));
	return true;
}

// ---------------------------------------------------------------------------
// RebuildLandscape
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRebuildLandscapeMetadataTest,
	"MCPServer.Unit.Landscape.RebuildLandscape.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRebuildLandscapeMetadataTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	FRebuildLandscapeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("rebuild_landscape"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRebuildLandscapeSuccessTest,
	"MCPServer.Unit.Landscape.RebuildLandscape.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRebuildLandscapeSuccessTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.RebuildLandscapeResult.bSuccess = true;
	FRebuildLandscapeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Landscape_0"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains rebuilt"), MCPTestUtils::GetResultText(Result).Contains(TEXT("rebuilt")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRebuildLandscapeModuleFailureTest,
	"MCPServer.Unit.Landscape.RebuildLandscape.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRebuildLandscapeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;
	Mock.RebuildLandscapeResult.bSuccess = false;
	Mock.RebuildLandscapeResult.ErrorMessage = TEXT("Landscape not found");
	FRebuildLandscapeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_name"), TEXT("Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Landscape not found")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
