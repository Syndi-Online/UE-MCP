// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockNavigationModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/BuildNavigationImplTool.h"
#include "Tools/Impl/ProjectPointToNavImplTool.h"
#include "Tools/Impl/FindPathImplTool.h"
#include "Tools/Impl/GetRandomReachablePointImplTool.h"
#include "Tools/Impl/RebuildNavigationImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNavigationIntegrationTest,
	"MCPServer.Integration.Navigation.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNavigationIntegrationTest::RunTest(const FString& Parameters)
{
	FMockNavigationModule Mock;

	// Step 1: Build navigation
	Mock.BuildNavigationResult.bSuccess = true;
	FBuildNavigationImplTool BuildTool(Mock);
	TestTrue(TEXT("BuildNavigation success"), MCPTestUtils::IsSuccess(BuildTool.Execute(MakeShared<FJsonObject>())));

	// Step 2: Project point to nav
	Mock.ProjectPointToNavResult.bSuccess = true;
	Mock.ProjectPointToNavResult.ProjectedLocation = FVector(100, 200, 0);
	FProjectPointToNavImplTool ProjectTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	auto Point = MakeShared<FJsonObject>();
	Point->SetNumberField(TEXT("x"), 100.0);
	Point->SetNumberField(TEXT("y"), 200.0);
	Point->SetNumberField(TEXT("z"), 50.0);
	Args->SetObjectField(TEXT("point"), Point);
	TestTrue(TEXT("ProjectPoint success"), MCPTestUtils::IsSuccess(ProjectTool.Execute(Args)));

	// Step 3: Find path
	Mock.FindPathResult.bSuccess = true;
	Mock.FindPathResult.bPartialPath = false;
	Mock.FindPathResult.PathPoints.Add(FVector(0, 0, 0));
	Mock.FindPathResult.PathPoints.Add(FVector(50, 50, 0));
	Mock.FindPathResult.PathPoints.Add(FVector(100, 100, 0));
	FFindPathImplTool PathTool(Mock);
	Args = MakeShared<FJsonObject>();
	auto Start = MakeShared<FJsonObject>();
	Start->SetNumberField(TEXT("x"), 0.0);
	Start->SetNumberField(TEXT("y"), 0.0);
	Start->SetNumberField(TEXT("z"), 0.0);
	auto End = MakeShared<FJsonObject>();
	End->SetNumberField(TEXT("x"), 100.0);
	End->SetNumberField(TEXT("y"), 100.0);
	End->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("start"), Start);
	Args->SetObjectField(TEXT("end"), End);
	TestTrue(TEXT("FindPath success"), MCPTestUtils::IsSuccess(PathTool.Execute(Args)));

	// Step 4: Get random reachable point
	Mock.GetRandomReachablePointResult.bSuccess = true;
	Mock.GetRandomReachablePointResult.Location = FVector(50, 50, 0);
	FGetRandomReachablePointImplTool RandomTool(Mock);
	Args = MakeShared<FJsonObject>();
	auto Origin = MakeShared<FJsonObject>();
	Origin->SetNumberField(TEXT("x"), 0.0);
	Origin->SetNumberField(TEXT("y"), 0.0);
	Origin->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("origin"), Origin);
	Args->SetNumberField(TEXT("radius"), 1000.0);
	TestTrue(TEXT("RandomPoint success"), MCPTestUtils::IsSuccess(RandomTool.Execute(Args)));

	// Step 5: Rebuild navigation
	Mock.RebuildNavigationResult.bSuccess = true;
	FRebuildNavigationImplTool RebuildTool(Mock);
	TestTrue(TEXT("RebuildNavigation success"), MCPTestUtils::IsSuccess(RebuildTool.Execute(MakeShared<FJsonObject>())));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 5);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
