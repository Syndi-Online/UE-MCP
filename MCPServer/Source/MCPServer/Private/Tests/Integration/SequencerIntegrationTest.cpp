// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockSequencerModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/SequencerSetPlaybackRangeImplTool.h"
#include "Tools/Impl/SequencerSetTimeImplTool.h"
#include "Tools/Impl/SequencerGetTimeImplTool.h"
#include "Tools/Impl/SequencerPlayImplTool.h"
#include "Tools/Impl/SequencerPauseImplTool.h"
#include "Tools/Impl/SequencerStopImplTool.h"
#include "Tools/Impl/SequencerAddActorImplTool.h"
#include "Tools/Impl/SequencerAddTrackImplTool.h"
#include "Tools/Impl/SequencerAddMarkerImplTool.h"
#include "Tools/Impl/SequencerForceEvaluateImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerIntegrationTest,
	"MCPServer.Integration.Sequencer.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerIntegrationTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;

	// Step 1: SequencerSetPlaybackRange
	Mock.SequencerSetPlaybackRangeResult.bSuccess = true;

	FSequencerSetPlaybackRangeImplTool RangeTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("start_time_seconds"), 0.0);
	Args->SetNumberField(TEXT("end_time_seconds"), 10.0);
	TestTrue(TEXT("SetPlaybackRange success"), MCPTestUtils::IsSuccess(RangeTool.Execute(Args)));

	// Step 2: SequencerSetTime (1.0)
	Mock.SequencerSetTimeResult.bSuccess = true;

	FSequencerSetTimeImplTool SetTimeTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("time_seconds"), 1.0);
	TestTrue(TEXT("SetTime success"), MCPTestUtils::IsSuccess(SetTimeTool.Execute(Args)));

	// Step 3: SequencerGetTime
	Mock.SequencerGetTimeResult.bSuccess = true;
	Mock.SequencerGetTimeResult.TimeInSeconds = 1.0;
	Mock.SequencerGetTimeResult.FrameNumber = 30;
	Mock.SequencerGetTimeResult.FrameRate = 30.0;

	FSequencerGetTimeImplTool GetTimeTool(Mock);
	Args = MakeShared<FJsonObject>();
	auto Result = GetTimeTool.Execute(Args);
	TestTrue(TEXT("GetTime success"), MCPTestUtils::IsSuccess(Result));

	// Step 4: SequencerPlay
	Mock.SequencerPlayResult.bSuccess = true;

	FSequencerPlayImplTool PlayTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("Play success"), MCPTestUtils::IsSuccess(PlayTool.Execute(Args)));

	// Step 5: SequencerPause
	Mock.SequencerPauseResult.bSuccess = true;

	FSequencerPauseImplTool PauseTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("Pause success"), MCPTestUtils::IsSuccess(PauseTool.Execute(Args)));

	// Step 6: SequencerStop
	Mock.SequencerStopResult.bSuccess = true;

	FSequencerStopImplTool StopTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("Stop success"), MCPTestUtils::IsSuccess(StopTool.Execute(Args)));

	// Step 7: SequencerAddActor
	Mock.SequencerAddActorResult.bSuccess = true;
	Mock.SequencerAddActorResult.BindingGuid = TEXT("ABC-123-DEF");

	FSequencerAddActorImplTool AddActorTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("PointLight_0"));
	Result = AddActorTool.Execute(Args);
	TestTrue(TEXT("AddActor success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("AddActor has binding guid"), MCPTestUtils::GetResultText(Result).Contains(TEXT("ABC-123-DEF")));

	// Step 8: SequencerAddTrack
	Mock.SequencerAddTrackResult.bSuccess = true;
	Mock.SequencerAddTrackResult.TrackName = TEXT("Transform");

	FSequencerAddTrackImplTool AddTrackTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("track_type"), TEXT("Transform"));
	Args->SetStringField(TEXT("binding_guid"), TEXT("ABC-123-DEF"));
	TestTrue(TEXT("AddTrack success"), MCPTestUtils::IsSuccess(AddTrackTool.Execute(Args)));

	// Step 9: SequencerAddMarker
	Mock.SequencerAddMarkerResult.bSuccess = true;
	Mock.SequencerAddMarkerResult.MarkerIndex = 0;

	FSequencerAddMarkerImplTool AddMarkerTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("time_seconds"), 5.0);
	Args->SetStringField(TEXT("label"), TEXT("KeyMoment"));
	TestTrue(TEXT("AddMarker success"), MCPTestUtils::IsSuccess(AddMarkerTool.Execute(Args)));

	// Step 10: SequencerForceEvaluate
	Mock.SequencerForceEvaluateResult.bSuccess = true;

	FSequencerForceEvaluateImplTool ForceEvalTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("ForceEvaluate success"), MCPTestUtils::IsSuccess(ForceEvalTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("SetPlaybackRange called"), Mock.Recorder.GetCallCount(TEXT("SequencerSetPlaybackRange")), 1);
	TestEqual(TEXT("SetTime called"), Mock.Recorder.GetCallCount(TEXT("SequencerSetTime")), 1);
	TestEqual(TEXT("GetTime called"), Mock.Recorder.GetCallCount(TEXT("SequencerGetTime")), 1);
	TestEqual(TEXT("Play called"), Mock.Recorder.GetCallCount(TEXT("SequencerPlay")), 1);
	TestEqual(TEXT("Pause called"), Mock.Recorder.GetCallCount(TEXT("SequencerPause")), 1);
	TestEqual(TEXT("Stop called"), Mock.Recorder.GetCallCount(TEXT("SequencerStop")), 1);
	TestEqual(TEXT("AddActor called"), Mock.Recorder.GetCallCount(TEXT("SequencerAddActor")), 1);
	TestEqual(TEXT("AddTrack called"), Mock.Recorder.GetCallCount(TEXT("SequencerAddTrack")), 1);
	TestEqual(TEXT("AddMarker called"), Mock.Recorder.GetCallCount(TEXT("SequencerAddMarker")), 1);
	TestEqual(TEXT("ForceEvaluate called"), Mock.Recorder.GetCallCount(TEXT("SequencerForceEvaluate")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 10);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
