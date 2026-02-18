// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/ISequencerToolModule.h"

class ISequencer;

class FSequencerToolImplModule : public ISequencerToolModule
{
public:
	virtual FSequencerSimpleResult SequencerPlay() override;
	virtual FSequencerSimpleResult SequencerPause() override;
	virtual FSequencerSimpleResult SequencerStop() override;
	virtual FSequencerSimpleResult SequencerSetTime(double TimeInSeconds) override;
	virtual FSequencerTimeResult SequencerGetTime() override;
	virtual FSequencerSimpleResult SequencerSetPlaybackRange(double StartTimeSeconds, double EndTimeSeconds) override;
	virtual FSequencerAddActorResult SequencerAddActor(const FString& ActorName) override;
	virtual FSequencerAddTrackResult SequencerAddTrack(const FString& TrackType, const FString& BindingGuid) override;
	virtual FSequencerAddMarkerResult SequencerAddMarker(double TimeInSeconds, const FString& Label) override;
	virtual FSequencerSimpleResult SequencerForceEvaluate() override;

private:
	TSharedPtr<ISequencer> GetActiveSequencer() const;
};
