// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockWorldPartitionModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/BuildHlodWpImplTool.h"
#include "Tools/Impl/DeleteHlodWpImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWorldPartitionIntegrationTest,
	"MCPServer.Integration.WorldPartition.HLODLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FWorldPartitionIntegrationTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;

	// Step 1: Build HLOD
	Mock.BuildHLODResult.bSuccess = true;
	Mock.BuildHLODResult.Message = TEXT("HLOD built successfully");
	FBuildHlodWpImplTool BuildTool(Mock);
	auto Result = BuildTool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("BuildHLOD success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: Delete HLOD
	Mock.DeleteHLODResult.bSuccess = true;
	Mock.DeleteHLODResult.Message = TEXT("HLOD deleted");
	FDeleteHlodWpImplTool DeleteTool(Mock);
	Result = DeleteTool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("DeleteHLOD success"), MCPTestUtils::IsSuccess(Result));

	TestEqual(TEXT("Build called"), Mock.Recorder.GetCallCount(TEXT("BuildHLOD")), 1);
	TestEqual(TEXT("Delete called"), Mock.Recorder.GetCallCount(TEXT("DeleteHLOD")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
