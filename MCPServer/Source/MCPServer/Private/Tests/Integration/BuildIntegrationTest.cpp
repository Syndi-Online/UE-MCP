// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockBuildModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/GetBuildStatusImplTool.h"
#include "Tools/Impl/BuildGeometryImplTool.h"
#include "Tools/Impl/BuildLightingImplTool.h"
#include "Tools/Impl/BuildAllImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildIntegrationTest,
	"MCPServer.Integration.Build.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildIntegrationTest::RunTest(const FString& Parameters)
{
	FMockBuildModule Mock;

	// Step 1: Get build status (not building)
	Mock.GetBuildStatusResult.bSuccess = true;
	Mock.GetBuildStatusResult.bIsBuilding = false;
	FGetBuildStatusImplTool StatusTool(Mock);
	auto Result = StatusTool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("GetBuildStatus success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: Build geometry
	Mock.BuildGeometryResult.bSuccess = true;
	FBuildGeometryImplTool GeoTool(Mock);
	TestTrue(TEXT("BuildGeometry success"), MCPTestUtils::IsSuccess(GeoTool.Execute(MakeShared<FJsonObject>())));

	// Step 3: Build lighting
	Mock.BuildLightingResult.bSuccess = true;
	FBuildLightingImplTool LightTool(Mock);
	TestTrue(TEXT("BuildLighting success"), MCPTestUtils::IsSuccess(LightTool.Execute(MakeShared<FJsonObject>())));

	// Step 4: Build all
	Mock.BuildAllResult.bSuccess = true;
	FBuildAllImplTool AllTool(Mock);
	TestTrue(TEXT("BuildAll success"), MCPTestUtils::IsSuccess(AllTool.Execute(MakeShared<FJsonObject>())));

	// Step 5: Get build status again
	Mock.GetBuildStatusResult.bIsBuilding = false;
	FGetBuildStatusImplTool StatusTool2(Mock);
	TestTrue(TEXT("GetBuildStatus2 success"), MCPTestUtils::IsSuccess(StatusTool2.Execute(MakeShared<FJsonObject>())));

	TestEqual(TEXT("GetBuildStatus calls"), Mock.Recorder.GetCallCount(TEXT("GetBuildStatus")), 2);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
