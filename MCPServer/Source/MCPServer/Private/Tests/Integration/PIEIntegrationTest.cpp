// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockPIEModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/IsPlayingImplTool.h"
#include "Tools/Impl/PlayInEditorImplTool.h"
#include "Tools/Impl/StopPieImplTool.h"
#include "Tools/Impl/SimulateInEditorImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPIEIntegrationPlayTest,
	"MCPServer.Integration.PIE.PlayStopCycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPIEIntegrationPlayTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;

	// Step 1: Verify not playing
	Mock.IsPlayingResult.bSuccess = true;
	Mock.IsPlayingResult.bIsPlaying = false;
	Mock.IsPlayingResult.bIsSimulating = false;
	FIsPlayingImplTool IsPlayTool(Mock);
	auto Result = IsPlayTool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsPlaying check success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: Play in editor
	Mock.PlayInEditorResult.bSuccess = true;
	Mock.PlayInEditorResult.Message = TEXT("PIE started");
	FPlayInEditorImplTool PlayTool(Mock);
	TestTrue(TEXT("PlayInEditor success"), MCPTestUtils::IsSuccess(PlayTool.Execute(MakeShared<FJsonObject>())));

	// Step 3: Verify playing
	Mock.IsPlayingResult.bIsPlaying = true;
	FIsPlayingImplTool IsPlayTool2(Mock);
	Result = IsPlayTool2.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsPlaying during PIE"), MCPTestUtils::IsSuccess(Result));

	// Step 4: Stop PIE
	Mock.StopPIEResult.bSuccess = true;
	FStopPieImplTool StopTool(Mock);
	TestTrue(TEXT("StopPIE success"), MCPTestUtils::IsSuccess(StopTool.Execute(MakeShared<FJsonObject>())));

	// Step 5: Verify stopped
	Mock.IsPlayingResult.bIsPlaying = false;
	FIsPlayingImplTool IsPlayTool3(Mock);
	Result = IsPlayTool3.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsPlaying after stop"), MCPTestUtils::IsSuccess(Result));

	TestEqual(TEXT("IsPlaying calls"), Mock.Recorder.GetCallCount(TEXT("IsPlaying")), 3);
	TestEqual(TEXT("PlayInEditor calls"), Mock.Recorder.GetCallCount(TEXT("PlayInEditor")), 1);
	TestEqual(TEXT("StopPIE calls"), Mock.Recorder.GetCallCount(TEXT("StopPIE")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPIEIntegrationSimulateTest,
	"MCPServer.Integration.PIE.SimulateCycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPIEIntegrationSimulateTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;

	// Step 1: Simulate
	Mock.SimulateInEditorResult.bSuccess = true;
	FSimulateInEditorImplTool SimTool(Mock);
	TestTrue(TEXT("Simulate success"), MCPTestUtils::IsSuccess(SimTool.Execute(MakeShared<FJsonObject>())));

	// Step 2: Check simulating
	Mock.IsPlayingResult.bSuccess = true;
	Mock.IsPlayingResult.bIsPlaying = true;
	Mock.IsPlayingResult.bIsSimulating = true;
	FIsPlayingImplTool IsPlayTool(Mock);
	TestTrue(TEXT("IsPlaying during simulate"), MCPTestUtils::IsSuccess(IsPlayTool.Execute(MakeShared<FJsonObject>())));

	// Step 3: Stop
	Mock.StopPIEResult.bSuccess = true;
	FStopPieImplTool StopTool(Mock);
	TestTrue(TEXT("StopPIE success"), MCPTestUtils::IsSuccess(StopTool.Execute(MakeShared<FJsonObject>())));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 3);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
