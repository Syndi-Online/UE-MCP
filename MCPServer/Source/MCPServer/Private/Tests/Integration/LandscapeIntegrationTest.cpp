// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockLandscapeModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/ImportLandscapeImplTool.h"
#include "Tools/Impl/GetLandscapeHeightmapImplTool.h"
#include "Tools/Impl/GetLandscapeWeightmapImplTool.h"
#include "Tools/Impl/ExportLandscapeImplTool.h"
#include "Tools/Impl/RebuildLandscapeImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLandscapeIntegrationTest,
	"MCPServer.Integration.Landscape.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLandscapeIntegrationTest::RunTest(const FString& Parameters)
{
	FMockLandscapeModule Mock;

	// Step 1: Import landscape
	Mock.ImportLandscapeResult.bSuccess = true;
	FImportLandscapeImplTool ImportTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("heightmap_file_path"), TEXT("/tmp/heightmap.png"));
	TestTrue(TEXT("ImportLandscape success"), MCPTestUtils::IsSuccess(ImportTool.Execute(Args)));

	// Step 2: Get heightmap
	Mock.GetLandscapeHeightmapResult.bSuccess = true;
	Mock.GetLandscapeHeightmapResult.Width = 1024;
	Mock.GetLandscapeHeightmapResult.Height = 1024;
	FGetLandscapeHeightmapImplTool GetHMTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_actor_name"), TEXT("Landscape_0"));
	auto Result = GetHMTool.Execute(Args);
	TestTrue(TEXT("GetHeightmap success"), MCPTestUtils::IsSuccess(Result));

	// Step 3: Get weightmap
	Mock.GetLandscapeWeightmapResult.bSuccess = true;
	Mock.GetLandscapeWeightmapResult.LayerNames.Add(TEXT("Grass"));
	Mock.GetLandscapeWeightmapResult.LayerNames.Add(TEXT("Dirt"));
	FGetLandscapeWeightmapImplTool GetWMTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_actor_name"), TEXT("Landscape_0"));
	TestTrue(TEXT("GetWeightmap success"), MCPTestUtils::IsSuccess(GetWMTool.Execute(Args)));

	// Step 4: Export landscape
	Mock.ExportLandscapeResult.bSuccess = true;
	Mock.ExportLandscapeResult.FilePath = TEXT("/tmp/exported_hm.png");
	FExportLandscapeImplTool ExportTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_actor_name"), TEXT("Landscape_0"));
	Args->SetStringField(TEXT("output_file_path"), TEXT("/tmp/exported_hm.png"));
	TestTrue(TEXT("ExportLandscape success"), MCPTestUtils::IsSuccess(ExportTool.Execute(Args)));

	// Step 5: Rebuild landscape
	Mock.RebuildLandscapeResult.bSuccess = true;
	FRebuildLandscapeImplTool RebuildTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("landscape_actor_name"), TEXT("Landscape_0"));
	TestTrue(TEXT("RebuildLandscape success"), MCPTestUtils::IsSuccess(RebuildTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("Import called"), Mock.Recorder.GetCallCount(TEXT("ImportLandscape")), 1);
	TestEqual(TEXT("GetHeightmap called"), Mock.Recorder.GetCallCount(TEXT("GetLandscapeHeightmap")), 1);
	TestEqual(TEXT("GetWeightmap called"), Mock.Recorder.GetCallCount(TEXT("GetLandscapeWeightmap")), 1);
	TestEqual(TEXT("Export called"), Mock.Recorder.GetCallCount(TEXT("ExportLandscape")), 1);
	TestEqual(TEXT("Rebuild called"), Mock.Recorder.GetCallCount(TEXT("RebuildLandscape")), 1);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
