// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/SequencerToolImplModule.h"
#include "ISequencer.h"
#include "LevelEditorSequencerIntegration.h"
#include "MovieScene.h"
#include "MovieSceneMarkedFrame.h"
#include "LevelSequence.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "Tracks/MovieSceneVisibilityTrack.h"
#include "EngineUtils.h"
#include "Editor.h"

TSharedPtr<ISequencer> FSequencerToolImplModule::GetActiveSequencer() const
{
	TArray<TWeakPtr<ISequencer>> Sequencers = FLevelEditorSequencerIntegration::Get().GetSequencers();
	for (auto& WeakSeq : Sequencers)
	{
		TSharedPtr<ISequencer> Seq = WeakSeq.Pin();
		if (Seq.IsValid())
		{
			return Seq;
		}
	}
	return nullptr;
}

FSequencerSimpleResult FSequencerToolImplModule::SequencerPlay()
{
	FSequencerSimpleResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	Sequencer->OnPlay(false);
	Result.bSuccess = true;
	return Result;
}

FSequencerSimpleResult FSequencerToolImplModule::SequencerPause()
{
	FSequencerSimpleResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	Sequencer->Pause();
	Result.bSuccess = true;
	return Result;
}

FSequencerSimpleResult FSequencerToolImplModule::SequencerStop()
{
	FSequencerSimpleResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	Sequencer->Pause();
	Sequencer->SetLocalTime(FFrameTime(FFrameNumber(0)), ESnapTimeMode::STM_None);
	Result.bSuccess = true;
	return Result;
}

FSequencerSimpleResult FSequencerToolImplModule::SequencerSetTime(double TimeInSeconds)
{
	FSequencerSimpleResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	FFrameRate TickRes = Sequencer->GetFocusedTickResolution();
	FFrameTime TickTime = TickRes.AsFrameTime(TimeInSeconds);
	Sequencer->SetLocalTime(TickTime, ESnapTimeMode::STM_None);

	Result.bSuccess = true;
	return Result;
}

FSequencerTimeResult FSequencerToolImplModule::SequencerGetTime()
{
	FSequencerTimeResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	FQualifiedFrameTime QFT = Sequencer->GetLocalTime();
	Result.TimeInSeconds = QFT.AsSeconds();

	FFrameRate DisplayRate = Sequencer->GetFocusedDisplayRate();
	FFrameNumber DisplayFrame = FFrameRate::TransformTime(QFT.Time, QFT.Rate, DisplayRate).RoundToFrame();
	Result.FrameNumber = DisplayFrame.Value;
	Result.FrameRate = DisplayRate.AsDecimal();

	Result.bSuccess = true;
	return Result;
}

FSequencerSimpleResult FSequencerToolImplModule::SequencerSetPlaybackRange(double StartTimeSeconds, double EndTimeSeconds)
{
	FSequencerSimpleResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	UMovieSceneSequence* Sequence = Sequencer->GetFocusedMovieSceneSequence();
	if (!Sequence)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No focused movie scene sequence found.");
		return Result;
	}

	UMovieScene* MovieScene = Sequence->GetMovieScene();
	if (!MovieScene)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No movie scene found.");
		return Result;
	}

	FFrameRate TickRes = MovieScene->GetTickResolution();
	FFrameNumber StartTick = TickRes.AsFrameTime(StartTimeSeconds).RoundToFrame();
	FFrameNumber EndTick = TickRes.AsFrameTime(EndTimeSeconds).RoundToFrame();

	MovieScene->SetPlaybackRange(TRange<FFrameNumber>(StartTick, EndTick));
	Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::TrackValueChanged);

	Result.bSuccess = true;
	return Result;
}

FSequencerAddActorResult FSequencerToolImplModule::SequencerAddActor(const FString& ActorName)
{
	FSequencerAddActorResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world found.");
		return Result;
	}

	AActor* FoundActor = nullptr;
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel().Equals(ActorName, ESearchCase::IgnoreCase) ||
			It->GetName().Equals(ActorName, ESearchCase::IgnoreCase))
		{
			FoundActor = *It;
			break;
		}
	}

	if (!FoundActor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorName);
		return Result;
	}

	TArray<TWeakObjectPtr<AActor>> ActorsToAdd;
	ActorsToAdd.Add(FoundActor);
	TArray<FGuid> Guids = Sequencer->AddActors(ActorsToAdd, true);

	if (Guids.Num() == 0)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add actor '%s' to sequencer."), *ActorName);
		return Result;
	}

	Result.bSuccess = true;
	Result.BindingGuid = Guids[0].ToString();
	return Result;
}

FSequencerAddTrackResult FSequencerToolImplModule::SequencerAddTrack(const FString& TrackType, const FString& BindingGuid)
{
	FSequencerAddTrackResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	UMovieSceneSequence* Sequence = Sequencer->GetFocusedMovieSceneSequence();
	if (!Sequence)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No focused movie scene sequence found.");
		return Result;
	}

	UMovieScene* MovieScene = Sequence->GetMovieScene();
	if (!MovieScene)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No movie scene found.");
		return Result;
	}

	UClass* TrackClass = nullptr;
	if (TrackType.Equals(TEXT("Transform"), ESearchCase::IgnoreCase))
	{
		TrackClass = UMovieScene3DTransformTrack::StaticClass();
	}
	else if (TrackType.Equals(TEXT("Visibility"), ESearchCase::IgnoreCase))
	{
		TrackClass = UMovieSceneVisibilityTrack::StaticClass();
	}
	else
	{
		TrackClass = LoadObject<UClass>(nullptr, *TrackType);
		if (!TrackClass)
		{
			TrackClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *TrackType));
		}
		if (!TrackClass || !TrackClass->IsChildOf(UMovieSceneTrack::StaticClass()))
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Unknown track type: %s. Supported: Transform, Visibility, or a full class path."), *TrackType);
			return Result;
		}
	}

	UMovieSceneTrack* NewTrack = nullptr;
	if (!BindingGuid.IsEmpty())
	{
		FGuid Guid;
		if (!FGuid::Parse(BindingGuid, Guid))
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Invalid binding GUID: %s"), *BindingGuid);
			return Result;
		}
		NewTrack = MovieScene->AddTrack(TrackClass, Guid);
	}
	else
	{
		NewTrack = MovieScene->AddTrack(TrackClass);
	}

	if (!NewTrack)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add track of type '%s'."), *TrackType);
		return Result;
	}

	Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);

	Result.bSuccess = true;
	Result.TrackName = NewTrack->GetName();
	return Result;
}

FSequencerAddMarkerResult FSequencerToolImplModule::SequencerAddMarker(double TimeInSeconds, const FString& Label)
{
	FSequencerAddMarkerResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	UMovieSceneSequence* Sequence = Sequencer->GetFocusedMovieSceneSequence();
	if (!Sequence)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No focused movie scene sequence found.");
		return Result;
	}

	UMovieScene* MovieScene = Sequence->GetMovieScene();
	if (!MovieScene)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No movie scene found.");
		return Result;
	}

	FFrameRate TickRes = MovieScene->GetTickResolution();
	FMovieSceneMarkedFrame Marker;
	Marker.FrameNumber = TickRes.AsFrameTime(TimeInSeconds).RoundToFrame();
	Marker.Label = Label;

	int32 MarkerIdx = MovieScene->AddMarkedFrame(Marker);
	MovieScene->SortMarkedFrames();
	Sequencer->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::TrackValueChanged);

	Result.bSuccess = true;
	Result.MarkerIndex = MarkerIdx;
	return Result;
}

FSequencerSimpleResult FSequencerToolImplModule::SequencerForceEvaluate()
{
	FSequencerSimpleResult Result;

	TSharedPtr<ISequencer> Sequencer = GetActiveSequencer();
	if (!Sequencer.IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No active sequencer found. Open a Level Sequence in the editor first.");
		return Result;
	}

	Sequencer->ForceEvaluate();
	Result.bSuccess = true;
	return Result;
}
