// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IViewportModule.h"
#include "MockCallRecorder.h"

class FMockViewportModule : public IViewportModule
{
public:
	FMockCallRecorder Recorder;

	FViewportCameraResult GetViewportCameraResult;
	FSetViewportCameraResult SetViewportCameraResult;
	FSimpleResult SetGameViewResult;
	FSimpleResult SetViewportRealtimeResult;
	FSimpleResult SetViewModeResult;
	FSimpleResult SetCameraFOVResult;
	FSimpleResult FocusOnActorsResult;
	FScreenshotResult TakeScreenshotResult;
	FSimpleResult PilotActorResult;
	FSimpleResult EjectPilotResult;
	FSimpleResult SetBookmarkResult;
	FSimpleResult JumpToBookmarkResult;

	virtual FViewportCameraResult GetViewportCamera() override { Recorder.RecordCall(TEXT("GetViewportCamera")); return GetViewportCameraResult; }
	virtual FSetViewportCameraResult SetViewportCamera(const FVector& Location, const FRotator& Rotation) override { Recorder.RecordCall(TEXT("SetViewportCamera")); return SetViewportCameraResult; }
	virtual FSimpleResult SetGameView(bool bEnabled) override { Recorder.RecordCall(TEXT("SetGameView")); return SetGameViewResult; }
	virtual FSimpleResult SetViewportRealtime(bool bEnabled) override { Recorder.RecordCall(TEXT("SetViewportRealtime")); return SetViewportRealtimeResult; }
	virtual FSimpleResult SetViewMode(const FString& ModeName) override { Recorder.RecordCall(TEXT("SetViewMode")); return SetViewModeResult; }
	virtual FSimpleResult SetCameraFOV(float FOV) override { Recorder.RecordCall(TEXT("SetCameraFOV")); return SetCameraFOVResult; }
	virtual FSimpleResult FocusOnActors(const TArray<FString>& ActorIdentifiers) override { Recorder.RecordCall(TEXT("FocusOnActors")); return FocusOnActorsResult; }
	virtual FScreenshotResult TakeScreenshot(const FString& Filename) override { Recorder.RecordCall(TEXT("TakeScreenshot")); return TakeScreenshotResult; }
	virtual FSimpleResult PilotActor(const FString& ActorIdentifier) override { Recorder.RecordCall(TEXT("PilotActor")); return PilotActorResult; }
	virtual FSimpleResult EjectPilot() override { Recorder.RecordCall(TEXT("EjectPilot")); return EjectPilotResult; }
	virtual FSimpleResult SetBookmark(int32 Index) override { Recorder.RecordCall(TEXT("SetBookmark")); return SetBookmarkResult; }
	virtual FSimpleResult JumpToBookmark(int32 Index) override { Recorder.RecordCall(TEXT("JumpToBookmark")); return JumpToBookmarkResult; }
};
