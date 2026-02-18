// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IViewportModule.h"

class IActorModule;

class FViewportImplModule : public IViewportModule
{
public:
	explicit FViewportImplModule(IActorModule& InActorModule);

	virtual FViewportCameraResult GetViewportCamera() override;
	virtual FSetViewportCameraResult SetViewportCamera(const FVector& Location, const FRotator& Rotation) override;
	virtual FSimpleResult SetGameView(bool bEnabled) override;
	virtual FSimpleResult SetViewportRealtime(bool bEnabled) override;
	virtual FSimpleResult SetViewMode(const FString& ModeName) override;
	virtual FSimpleResult SetCameraFOV(float FOV) override;
	virtual FSimpleResult FocusOnActors(const TArray<FString>& ActorIdentifiers) override;
	virtual FScreenshotResult TakeScreenshot(const FString& Filename) override;
	virtual FSimpleResult PilotActor(const FString& ActorIdentifier) override;
	virtual FSimpleResult EjectPilot() override;
	virtual FSimpleResult SetBookmark(int32 Index) override;
	virtual FSimpleResult JumpToBookmark(int32 Index) override;

private:
	IActorModule& ActorModule;
};
