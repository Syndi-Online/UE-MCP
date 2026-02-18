// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FSequencerSimpleResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FSequencerTimeResult
{
	bool bSuccess = false;
	double TimeInSeconds = 0.0;
	int32 FrameNumber = 0;
	double FrameRate = 0.0;
	FString ErrorMessage;
};

struct FSequencerAddActorResult
{
	bool bSuccess = false;
	FString BindingGuid;
	FString ErrorMessage;
};

struct FSequencerAddTrackResult
{
	bool bSuccess = false;
	FString TrackName;
	FString ErrorMessage;
};

struct FSequencerAddMarkerResult
{
	bool bSuccess = false;
	int32 MarkerIndex = -1;
	FString ErrorMessage;
};

/**
 * Module interface for Sequencer / Cinematics operations.
 */
class ISequencerToolModule
{
public:
	virtual ~ISequencerToolModule() = default;

	virtual FSequencerSimpleResult SequencerPlay() = 0;
	virtual FSequencerSimpleResult SequencerPause() = 0;
	virtual FSequencerSimpleResult SequencerStop() = 0;
	virtual FSequencerSimpleResult SequencerSetTime(double TimeInSeconds) = 0;
	virtual FSequencerTimeResult SequencerGetTime() = 0;
	virtual FSequencerSimpleResult SequencerSetPlaybackRange(double StartTimeSeconds, double EndTimeSeconds) = 0;
	virtual FSequencerAddActorResult SequencerAddActor(const FString& ActorName) = 0;
	virtual FSequencerAddTrackResult SequencerAddTrack(const FString& TrackType, const FString& BindingGuid) = 0;
	virtual FSequencerAddMarkerResult SequencerAddMarker(double TimeInSeconds, const FString& Label) = 0;
	virtual FSequencerSimpleResult SequencerForceEvaluate() = 0;
};
