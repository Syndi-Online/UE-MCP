// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FViewportCameraResult
{
	bool bSuccess = false;
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	float FOV = 90.0f;
	FString ErrorMessage;
};

struct FSetViewportCameraResult
{
	bool bSuccess = false;
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FString ErrorMessage;
};

struct FSimpleResult
{
	bool bSuccess = false;
	FString Message;
	FString ErrorMessage;
};

struct FScreenshotResult
{
	bool bSuccess = false;
	FString FilePath;
	FString ErrorMessage;
};

/**
 * Module interface for viewport and camera operations.
 */
class IViewportModule
{
public:
	virtual ~IViewportModule() = default;

	virtual FViewportCameraResult GetViewportCamera() = 0;
	virtual FSetViewportCameraResult SetViewportCamera(const FVector& Location, const FRotator& Rotation) = 0;
	virtual FSimpleResult SetGameView(bool bEnabled) = 0;
	virtual FSimpleResult SetViewportRealtime(bool bEnabled) = 0;
	virtual FSimpleResult SetViewMode(const FString& ModeName) = 0;
	virtual FSimpleResult SetCameraFOV(float FOV) = 0;
	virtual FSimpleResult FocusOnActors(const TArray<FString>& ActorIdentifiers) = 0;
	virtual FScreenshotResult TakeScreenshot(const FString& Filename) = 0;
	virtual FSimpleResult PilotActor(const FString& ActorIdentifier) = 0;
	virtual FSimpleResult EjectPilot() = 0;
	virtual FSimpleResult SetBookmark(int32 Index) = 0;
	virtual FSimpleResult JumpToBookmark(int32 Index) = 0;
};
