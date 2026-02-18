// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/BuildNavigationImplTool.h"
#include "Tools/Impl/RebuildNavigationImplTool.h"
#include "Tools/Impl/FindPathImplTool.h"
#include "Tools/Impl/ProjectPointToNavImplTool.h"
#include "Tools/Impl/GetRandomReachablePointImplTool.h"
#include "Tests/Mocks/MockNavigationModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// BuildNavigation
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildNavigationMetadataTest,
	"MCPServer.Unit.Navigation.BuildNavigation.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildNavigationMetadataTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	FBuildNavigationImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("build_navigation"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildNavigationEmptyArgsTest,
	"MCPServer.Unit.Navigation.BuildNavigation.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildNavigationEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.BuildNavigationResult.bSuccess = true;

	FBuildNavigationImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildNavigation")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBuildNavigationModuleFailureTest,
	"MCPServer.Unit.Navigation.BuildNavigation.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FBuildNavigationModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.BuildNavigationResult.bSuccess = false;
	Mock.BuildNavigationResult.ErrorMessage = TEXT("No NavMesh bounds");

	FBuildNavigationImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("BuildNavigation")), 1);
	return true;
}

// ============================================================================
// RebuildNavigation
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRebuildNavigationMetadataTest,
	"MCPServer.Unit.Navigation.RebuildNavigation.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRebuildNavigationMetadataTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	FRebuildNavigationImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("rebuild_navigation"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRebuildNavigationEmptyArgsTest,
	"MCPServer.Unit.Navigation.RebuildNavigation.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRebuildNavigationEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.RebuildNavigationResult.bSuccess = true;

	FRebuildNavigationImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RebuildNavigation")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRebuildNavigationModuleFailureTest,
	"MCPServer.Unit.Navigation.RebuildNavigation.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRebuildNavigationModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.RebuildNavigationResult.bSuccess = false;
	Mock.RebuildNavigationResult.ErrorMessage = TEXT("Rebuild failed");

	FRebuildNavigationImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RebuildNavigation")), 1);
	return true;
}

// ============================================================================
// FindPath
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindPathMetadataTest,
	"MCPServer.Unit.Navigation.FindPath.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindPathMetadataTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	FFindPathImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("find_path"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindPathSuccessTest,
	"MCPServer.Unit.Navigation.FindPath.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindPathSuccessTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.FindPathResult.bSuccess = true;
	Mock.FindPathResult.bPartialPath = false;
	Mock.FindPathResult.PathPoints.Add(FVector(0, 0, 0));
	Mock.FindPathResult.PathPoints.Add(FVector(50, 50, 0));
	Mock.FindPathResult.PathPoints.Add(FVector(100, 100, 0));

	FFindPathImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> StartObj = MakeShared<FJsonObject>();
	StartObj->SetNumberField(TEXT("x"), 0.0);
	StartObj->SetNumberField(TEXT("y"), 0.0);
	StartObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("start"), StartObj);

	TSharedPtr<FJsonObject> EndObj = MakeShared<FJsonObject>();
	EndObj->SetNumberField(TEXT("x"), 100.0);
	EndObj->SetNumberField(TEXT("y"), 100.0);
	EndObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("end"), EndObj);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("FindPath")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindPathMissingArgsTest,
	"MCPServer.Unit.Navigation.FindPath.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindPathMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.FindPathResult.bSuccess = true;

	FFindPathImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// start/end default to zero vectors when missing, so module is still called
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("FindPath")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFindPathModuleFailureTest,
	"MCPServer.Unit.Navigation.FindPath.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFindPathModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.FindPathResult.bSuccess = false;
	Mock.FindPathResult.ErrorMessage = TEXT("No path found");

	FFindPathImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> StartObj = MakeShared<FJsonObject>();
	StartObj->SetNumberField(TEXT("x"), 0.0);
	StartObj->SetNumberField(TEXT("y"), 0.0);
	StartObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("start"), StartObj);

	TSharedPtr<FJsonObject> EndObj = MakeShared<FJsonObject>();
	EndObj->SetNumberField(TEXT("x"), 99999.0);
	EndObj->SetNumberField(TEXT("y"), 99999.0);
	EndObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("end"), EndObj);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("FindPath")), 1);
	return true;
}

// ============================================================================
// ProjectPointToNav
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectPointToNavMetadataTest,
	"MCPServer.Unit.Navigation.ProjectPointToNav.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectPointToNavMetadataTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	FProjectPointToNavImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("project_point_to_nav"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectPointToNavSuccessTest,
	"MCPServer.Unit.Navigation.ProjectPointToNav.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectPointToNavSuccessTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.ProjectPointToNavResult.bSuccess = true;
	Mock.ProjectPointToNavResult.ProjectedLocation = FVector(100, 200, 0);

	FProjectPointToNavImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PointObj = MakeShared<FJsonObject>();
	PointObj->SetNumberField(TEXT("x"), 100.0);
	PointObj->SetNumberField(TEXT("y"), 200.0);
	PointObj->SetNumberField(TEXT("z"), 50.0);
	Args->SetObjectField(TEXT("point"), PointObj);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ProjectPointToNav")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectPointToNavMissingArgsTest,
	"MCPServer.Unit.Navigation.ProjectPointToNav.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectPointToNavMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.ProjectPointToNavResult.bSuccess = true;

	FProjectPointToNavImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// point defaults to zero vector when missing, so module is still called
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ProjectPointToNav")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectPointToNavModuleFailureTest,
	"MCPServer.Unit.Navigation.ProjectPointToNav.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectPointToNavModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.ProjectPointToNavResult.bSuccess = false;
	Mock.ProjectPointToNavResult.ErrorMessage = TEXT("Point not on navmesh");

	FProjectPointToNavImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PointObj = MakeShared<FJsonObject>();
	PointObj->SetNumberField(TEXT("x"), 99999.0);
	PointObj->SetNumberField(TEXT("y"), 99999.0);
	PointObj->SetNumberField(TEXT("z"), 99999.0);
	Args->SetObjectField(TEXT("point"), PointObj);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ProjectPointToNav")), 1);
	return true;
}

// ============================================================================
// GetRandomReachablePoint
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetRandomReachablePointMetadataTest,
	"MCPServer.Unit.Navigation.GetRandomReachablePoint.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetRandomReachablePointMetadataTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	FGetRandomReachablePointImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_random_reachable_point"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetRandomReachablePointSuccessTest,
	"MCPServer.Unit.Navigation.GetRandomReachablePoint.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetRandomReachablePointSuccessTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.GetRandomReachablePointResult.bSuccess = true;
	Mock.GetRandomReachablePointResult.Location = FVector(50, 50, 0);

	FGetRandomReachablePointImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> OriginObj = MakeShared<FJsonObject>();
	OriginObj->SetNumberField(TEXT("x"), 0.0);
	OriginObj->SetNumberField(TEXT("y"), 0.0);
	OriginObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("origin"), OriginObj);
	Args->SetNumberField(TEXT("radius"), 1000.0);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetRandomReachablePoint")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetRandomReachablePointMissingArgsTest,
	"MCPServer.Unit.Navigation.GetRandomReachablePoint.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetRandomReachablePointMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.GetRandomReachablePointResult.bSuccess = true;

	FGetRandomReachablePointImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// origin/radius default when missing, so module is still called
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetRandomReachablePoint")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetRandomReachablePointModuleFailureTest,
	"MCPServer.Unit.Navigation.GetRandomReachablePoint.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetRandomReachablePointModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;
	Mock.GetRandomReachablePointResult.bSuccess = false;
	Mock.GetRandomReachablePointResult.ErrorMessage = TEXT("No reachable point");

	FGetRandomReachablePointImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> OriginObj = MakeShared<FJsonObject>();
	OriginObj->SetNumberField(TEXT("x"), 0.0);
	OriginObj->SetNumberField(TEXT("y"), 0.0);
	OriginObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("origin"), OriginObj);
	Args->SetNumberField(TEXT("radius"), 10.0);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetRandomReachablePoint")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
