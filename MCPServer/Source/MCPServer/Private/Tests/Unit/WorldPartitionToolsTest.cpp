// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/BuildHlodWpImplTool.h"
#include "Tools/Impl/DeleteHlodWpImplTool.h"
#include "Tests/Mocks/MockWorldPartitionModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// BuildHLODWP
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODWPMetadataTest,
	"MCPServer.Unit.WorldPartition.BuildHLODWP.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODWPMetadataTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	FBuildHlodWpImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_hlod_wp"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODWPSuccessTest,
	"MCPServer.Unit.WorldPartition.BuildHLODWP.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODWPSuccessTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	Mock.BuildHLODResult.bSuccess = true;
	Mock.BuildHLODResult.Message = TEXT("HLOD built");

	FBuildHlodWpImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildHLOD")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODWPEmptyArgsTest,
	"MCPServer.Unit.WorldPartition.BuildHLODWP.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODWPEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	Mock.BuildHLODResult.bSuccess = true;

	FBuildHlodWpImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildHLOD")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildHLODWPModuleFailureTest,
	"MCPServer.Unit.WorldPartition.BuildHLODWP.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildHLODWPModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	Mock.BuildHLODResult.bSuccess = false;
	Mock.BuildHLODResult.ErrorMessage = TEXT("World Partition not enabled");

	FBuildHlodWpImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildHLOD")), 1);
	return true;
}

// ============================================================================
// DeleteHLODWP
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteHLODWPMetadataTest,
	"MCPServer.Unit.WorldPartition.DeleteHLODWP.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteHLODWPMetadataTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	FDeleteHlodWpImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("delete_hlod_wp"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteHLODWPSuccessTest,
	"MCPServer.Unit.WorldPartition.DeleteHLODWP.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteHLODWPSuccessTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	Mock.DeleteHLODResult.bSuccess = true;
	Mock.DeleteHLODResult.Message = TEXT("HLOD deleted");

	FDeleteHlodWpImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("DeleteHLOD")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteHLODWPEmptyArgsTest,
	"MCPServer.Unit.WorldPartition.DeleteHLODWP.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteHLODWPEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	Mock.DeleteHLODResult.bSuccess = true;

	FDeleteHlodWpImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("DeleteHLOD")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteHLODWPModuleFailureTest,
	"MCPServer.Unit.WorldPartition.DeleteHLODWP.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteHLODWPModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockWorldPartitionModule Mock;
	Mock.DeleteHLODResult.bSuccess = false;
	Mock.DeleteHLODResult.ErrorMessage = TEXT("No HLOD data to delete");

	FDeleteHlodWpImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("DeleteHLOD")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
