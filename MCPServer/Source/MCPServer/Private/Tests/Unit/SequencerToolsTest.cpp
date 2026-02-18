// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/SequencerPlayImplTool.h"
#include "Tools/Impl/SequencerPauseImplTool.h"
#include "Tools/Impl/SequencerStopImplTool.h"
#include "Tools/Impl/SequencerSetTimeImplTool.h"
#include "Tools/Impl/SequencerGetTimeImplTool.h"
#include "Tools/Impl/SequencerSetPlaybackRangeImplTool.h"
#include "Tools/Impl/SequencerAddActorImplTool.h"
#include "Tools/Impl/SequencerAddTrackImplTool.h"
#include "Tools/Impl/SequencerAddMarkerImplTool.h"
#include "Tools/Impl/SequencerForceEvaluateImplTool.h"
#include "Tests/Mocks/MockSequencerModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ---------------------------------------------------------------------------
// SequencerPlay
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerPlayMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerPlay.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerPlayMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerPlayImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_play"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerPlaySuccessTest,
	"MCPServer.Unit.Sequencer.SequencerPlay.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerPlaySuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerPlayResult.bSuccess = true;
	FSequencerPlayImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains started"), MCPTestUtils::GetResultText(Result).Contains(TEXT("started")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerPlayModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerPlay.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerPlayModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerPlayResult.bSuccess = false;
	Mock.SequencerPlayResult.ErrorMessage = TEXT("No sequence open");
	FSequencerPlayImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerPause
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerPauseMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerPause.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerPauseMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerPauseImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_pause"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerPauseSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerPause.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerPauseSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerPauseResult.bSuccess = true;
	FSequencerPauseImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains paused"), MCPTestUtils::GetResultText(Result).Contains(TEXT("paused")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerPauseModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerPause.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerPauseModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerPauseResult.bSuccess = false;
	Mock.SequencerPauseResult.ErrorMessage = TEXT("No sequence open");
	FSequencerPauseImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerStop
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerStopMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerStop.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerStopMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerStopImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_stop"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerStopSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerStop.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerStopSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerStopResult.bSuccess = true;
	FSequencerStopImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerStopModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerStop.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerStopModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerStopResult.bSuccess = false;
	Mock.SequencerStopResult.ErrorMessage = TEXT("No sequence open");
	FSequencerStopImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerSetTime
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetTimeMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerSetTime.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetTimeMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerSetTimeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_set_time"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetTimeSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerSetTime.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetTimeSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerSetTimeResult.bSuccess = true;
	FSequencerSetTimeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("time_seconds"), 2.5);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2.500"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2.500")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetTimeMissingArgsTest,
	"MCPServer.Unit.Sequencer.SequencerSetTime.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetTimeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerSetTimeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions time_seconds"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("time_seconds")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetTimeModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerSetTime.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetTimeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerSetTimeResult.bSuccess = false;
	Mock.SequencerSetTimeResult.ErrorMessage = TEXT("No sequence open");
	FSequencerSetTimeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("time_seconds"), 2.5);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerGetTime
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerGetTimeMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerGetTime.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerGetTimeMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerGetTimeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_get_time"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerGetTimeSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerGetTime.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerGetTimeSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerGetTimeResult.bSuccess = true;
	Mock.SequencerGetTimeResult.TimeInSeconds = 2.5;
	Mock.SequencerGetTimeResult.FrameNumber = 75;
	FSequencerGetTimeImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 2.5"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2.5")));
	TestTrue(TEXT("Contains 75"), MCPTestUtils::GetResultText(Result).Contains(TEXT("75")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerGetTimeModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerGetTime.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerGetTimeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerGetTimeResult.bSuccess = false;
	Mock.SequencerGetTimeResult.ErrorMessage = TEXT("No sequence open");
	FSequencerGetTimeImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerSetPlaybackRange
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetPlaybackRangeMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerSetPlaybackRange.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetPlaybackRangeMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerSetPlaybackRangeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_set_playback_range"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetPlaybackRangeSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerSetPlaybackRange.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetPlaybackRangeSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerSetPlaybackRangeResult.bSuccess = true;
	FSequencerSetPlaybackRangeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("start_time_seconds"), 0.0);
	Args->SetNumberField(TEXT("end_time_seconds"), 10.0);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains range"), MCPTestUtils::GetResultText(Result).Contains(TEXT("range")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetPlaybackRangeMissingArgsTest,
	"MCPServer.Unit.Sequencer.SequencerSetPlaybackRange.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetPlaybackRangeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerSetPlaybackRangeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions required params"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("start_time_seconds")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerSetPlaybackRangeModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerSetPlaybackRange.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerSetPlaybackRangeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerSetPlaybackRangeResult.bSuccess = false;
	Mock.SequencerSetPlaybackRangeResult.ErrorMessage = TEXT("Invalid range");
	FSequencerSetPlaybackRangeImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("start_time_seconds"), 10.0);
	Args->SetNumberField(TEXT("end_time_seconds"), 0.0);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid range")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerAddActor
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddActorMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerAddActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerAddActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_add_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddActorSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerAddActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerAddActorResult.bSuccess = true;
	Mock.SequencerAddActorResult.BindingGuid = TEXT("abc-123");
	FSequencerAddActorImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("MyActor"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains abc-123"), MCPTestUtils::GetResultText(Result).Contains(TEXT("abc-123")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddActorMissingArgsTest,
	"MCPServer.Unit.Sequencer.SequencerAddActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerAddActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions actor_name"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("actor_name")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddActorModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerAddActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerAddActorResult.bSuccess = false;
	Mock.SequencerAddActorResult.ErrorMessage = TEXT("Actor not found");
	FSequencerAddActorImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("MissingActor"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Actor not found")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerAddTrack
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddTrackMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerAddTrack.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddTrackMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerAddTrackImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_add_track"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddTrackSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerAddTrack.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddTrackSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerAddTrackResult.bSuccess = true;
	Mock.SequencerAddTrackResult.TrackName = TEXT("Transform");
	FSequencerAddTrackImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("track_type"), TEXT("Transform"));
	Args->SetStringField(TEXT("binding_guid"), TEXT("abc-123"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Transform"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Transform")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddTrackMissingArgsTest,
	"MCPServer.Unit.Sequencer.SequencerAddTrack.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddTrackMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerAddTrackImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions track_type"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("track_type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddTrackModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerAddTrack.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddTrackModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerAddTrackResult.bSuccess = false;
	Mock.SequencerAddTrackResult.ErrorMessage = TEXT("Invalid track type");
	FSequencerAddTrackImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("track_type"), TEXT("BadType"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid track type")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerAddMarker
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddMarkerMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerAddMarker.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddMarkerMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerAddMarkerImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_add_marker"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddMarkerSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerAddMarker.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddMarkerSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerAddMarkerResult.bSuccess = true;
	Mock.SequencerAddMarkerResult.MarkerIndex = 0;
	FSequencerAddMarkerImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("time_seconds"), 1.5);
	Args->SetStringField(TEXT("label"), TEXT("Explosion"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Explosion"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Explosion")));
	TestTrue(TEXT("Contains index 0"), MCPTestUtils::GetResultText(Result).Contains(TEXT("0")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddMarkerMissingArgsTest,
	"MCPServer.Unit.Sequencer.SequencerAddMarker.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddMarkerMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerAddMarkerImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions time_seconds"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("time_seconds")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerAddMarkerModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerAddMarker.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerAddMarkerModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerAddMarkerResult.bSuccess = false;
	Mock.SequencerAddMarkerResult.ErrorMessage = TEXT("No sequence open");
	FSequencerAddMarkerImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("time_seconds"), 1.5);
	Args->SetStringField(TEXT("label"), TEXT("Marker"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

// ---------------------------------------------------------------------------
// SequencerForceEvaluate
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerForceEvaluateMetadataTest,
	"MCPServer.Unit.Sequencer.SequencerForceEvaluate.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerForceEvaluateMetadataTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	FSequencerForceEvaluateImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("sequencer_force_evaluate"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerForceEvaluateSuccessTest,
	"MCPServer.Unit.Sequencer.SequencerForceEvaluate.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerForceEvaluateSuccessTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerForceEvaluateResult.bSuccess = true;
	FSequencerForceEvaluateImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains force-evaluated"), MCPTestUtils::GetResultText(Result).Contains(TEXT("force-evaluated")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSequencerForceEvaluateModuleFailureTest,
	"MCPServer.Unit.Sequencer.SequencerForceEvaluate.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSequencerForceEvaluateModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSequencerModule Mock;
	Mock.SequencerForceEvaluateResult.bSuccess = false;
	Mock.SequencerForceEvaluateResult.ErrorMessage = TEXT("No sequence open");
	FSequencerForceEvaluateImplTool Tool(Mock);

	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No sequence open")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
