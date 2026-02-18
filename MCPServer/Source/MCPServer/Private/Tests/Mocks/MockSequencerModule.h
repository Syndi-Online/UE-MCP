// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ISequencerToolModule.h"
#include "MockCallRecorder.h"

class FMockSequencerModule : public ISequencerToolModule
{
public:
	FMockCallRecorder Recorder;

	FSequencerSimpleResult SequencerPlayResult;
	FSequencerSimpleResult SequencerPauseResult;
	FSequencerSimpleResult SequencerStopResult;
	FSequencerSimpleResult SequencerSetTimeResult;
	FSequencerTimeResult SequencerGetTimeResult;
	FSequencerSimpleResult SequencerSetPlaybackRangeResult;
	FSequencerAddActorResult SequencerAddActorResult;
	FSequencerAddTrackResult SequencerAddTrackResult;
	FSequencerAddMarkerResult SequencerAddMarkerResult;
	FSequencerSimpleResult SequencerForceEvaluateResult;

	virtual FSequencerSimpleResult SequencerPlay() override { Recorder.RecordCall(TEXT("SequencerPlay")); return SequencerPlayResult; }
	virtual FSequencerSimpleResult SequencerPause() override { Recorder.RecordCall(TEXT("SequencerPause")); return SequencerPauseResult; }
	virtual FSequencerSimpleResult SequencerStop() override { Recorder.RecordCall(TEXT("SequencerStop")); return SequencerStopResult; }
	virtual FSequencerSimpleResult SequencerSetTime(double TimeInSeconds) override { Recorder.RecordCall(TEXT("SequencerSetTime")); return SequencerSetTimeResult; }
	virtual FSequencerTimeResult SequencerGetTime() override { Recorder.RecordCall(TEXT("SequencerGetTime")); return SequencerGetTimeResult; }
	virtual FSequencerSimpleResult SequencerSetPlaybackRange(double StartTimeSeconds, double EndTimeSeconds) override { Recorder.RecordCall(TEXT("SequencerSetPlaybackRange")); return SequencerSetPlaybackRangeResult; }
	virtual FSequencerAddActorResult SequencerAddActor(const FString& ActorName) override { Recorder.RecordCall(TEXT("SequencerAddActor")); return SequencerAddActorResult; }
	virtual FSequencerAddTrackResult SequencerAddTrack(const FString& TrackType, const FString& BindingGuid) override { Recorder.RecordCall(TEXT("SequencerAddTrack")); return SequencerAddTrackResult; }
	virtual FSequencerAddMarkerResult SequencerAddMarker(double TimeInSeconds, const FString& Label) override { Recorder.RecordCall(TEXT("SequencerAddMarker")); return SequencerAddMarkerResult; }
	virtual FSequencerSimpleResult SequencerForceEvaluate() override { Recorder.RecordCall(TEXT("SequencerForceEvaluate")); return SequencerForceEvaluateResult; }
};
