// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/BuildGeometryImplTool.h"
#include "Tools/Impl/BuildLightingImplTool.h"
#include "Tools/Impl/BuildNavigationDataImplTool.h"
#include "Tools/Impl/BuildHlodImplTool.h"
#include "Tools/Impl/BuildTextureStreamingImplTool.h"
#include "Tools/Impl/BuildAllImplTool.h"
#include "Tools/Impl/GetBuildStatusImplTool.h"
#include "Tests/Mocks/MockBuildModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// BuildGeometry
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildGeometryMetadataTest,
	"MCPServer.Unit.Build.BuildGeometry.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildGeometryMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FBuildGeometryImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_geometry"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildGeometryEmptyArgsTest,
	"MCPServer.Unit.Build.BuildGeometry.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildGeometryEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildGeometryResult.bSuccess = true;

	FBuildGeometryImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildGeometry")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildGeometryModuleFailureTest,
	"MCPServer.Unit.Build.BuildGeometry.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildGeometryModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildGeometryResult.bSuccess = false;
	Mock.BuildGeometryResult.ErrorMessage = TEXT("Build failed");

	FBuildGeometryImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildGeometry")), 1);
	return true;
}

// ============================================================================
// BuildLighting
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildLightingMetadataTest,
	"MCPServer.Unit.Build.BuildLighting.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildLightingMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FBuildLightingImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_lighting"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildLightingEmptyArgsTest,
	"MCPServer.Unit.Build.BuildLighting.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildLightingEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildLightingResult.bSuccess = true;

	FBuildLightingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildLighting")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildLightingModuleFailureTest,
	"MCPServer.Unit.Build.BuildLighting.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildLightingModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildLightingResult.bSuccess = false;
	Mock.BuildLightingResult.ErrorMessage = TEXT("Lighting build failed");

	FBuildLightingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildLighting")), 1);
	return true;
}

// ============================================================================
// BuildNavigationData
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildNavigationDataMetadataTest,
	"MCPServer.Unit.Build.BuildNavigationData.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildNavigationDataMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FBuildNavigationDataImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_navigation_data"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildNavigationDataEmptyArgsTest,
	"MCPServer.Unit.Build.BuildNavigationData.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildNavigationDataEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildNavigationDataResult.bSuccess = true;

	FBuildNavigationDataImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildNavigationData")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildNavigationDataModuleFailureTest,
	"MCPServer.Unit.Build.BuildNavigationData.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildNavigationDataModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildNavigationDataResult.bSuccess = false;
	Mock.BuildNavigationDataResult.ErrorMessage = TEXT("Navigation build failed");

	FBuildNavigationDataImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildNavigationData")), 1);
	return true;
}

// ============================================================================
// BuildHLOD
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODMetadataTest,
	"MCPServer.Unit.Build.BuildHLOD.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FBuildHlodImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_hlod"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODEmptyArgsTest,
	"MCPServer.Unit.Build.BuildHLOD.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildHLODResult.bSuccess = true;

	FBuildHlodImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildHLOD")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODModuleFailureTest,
	"MCPServer.Unit.Build.BuildHLOD.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildHLODResult.bSuccess = false;
	Mock.BuildHLODResult.ErrorMessage = TEXT("HLOD build failed");

	FBuildHlodImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildHLOD")), 1);
	return true;
}

// ============================================================================
// BuildTextureStreaming
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildTextureStreamingMetadataTest,
	"MCPServer.Unit.Build.BuildTextureStreaming.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildTextureStreamingMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FBuildTextureStreamingImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_texture_streaming"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildTextureStreamingEmptyArgsTest,
	"MCPServer.Unit.Build.BuildTextureStreaming.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildTextureStreamingEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildTextureStreamingResult.bSuccess = true;

	FBuildTextureStreamingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildTextureStreaming")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildTextureStreamingModuleFailureTest,
	"MCPServer.Unit.Build.BuildTextureStreaming.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildTextureStreamingModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildTextureStreamingResult.bSuccess = false;
	Mock.BuildTextureStreamingResult.ErrorMessage = TEXT("Texture streaming build failed");

	FBuildTextureStreamingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildTextureStreaming")), 1);
	return true;
}

// ============================================================================
// BuildAll
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildAllMetadataTest,
	"MCPServer.Unit.Build.BuildAll.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildAllMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FBuildAllImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_all"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildAllEmptyArgsTest,
	"MCPServer.Unit.Build.BuildAll.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildAllEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildAllResult.bSuccess = true;

	FBuildAllImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildAll")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildAllModuleFailureTest,
	"MCPServer.Unit.Build.BuildAll.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildAllModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.BuildAllResult.bSuccess = false;
	Mock.BuildAllResult.ErrorMessage = TEXT("Build all failed");

	FBuildAllImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildAll")), 1);
	return true;
}

// ============================================================================
// GetBuildStatus
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBuildStatusMetadataTest,
	"MCPServer.Unit.Build.GetBuildStatus.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBuildStatusMetadataTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	FGetBuildStatusImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_build_status"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBuildStatusSuccessTest,
	"MCPServer.Unit.Build.GetBuildStatus.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBuildStatusSuccessTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.GetBuildStatusResult.bSuccess = true;
	Mock.GetBuildStatusResult.bIsBuilding = false;
	Mock.GetBuildStatusResult.bIsLightingBuilding = false;

	FGetBuildStatusImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetBuildStatus")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetBuildStatusModuleFailureTest,
	"MCPServer.Unit.Build.GetBuildStatus.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetBuildStatusModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;
	Mock.GetBuildStatusResult.bSuccess = false;
	Mock.GetBuildStatusResult.ErrorMessage = TEXT("Could not get build status");

	FGetBuildStatusImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	// get_build_status always returns isError=false (status report, not an error)
	TestTrue(TEXT("Always succeeds"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetBuildStatus")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
