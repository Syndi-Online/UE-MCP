// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/INavigationModule.h"
#include "MockCallRecorder.h"

class FMockNavigationModule : public INavigationModule
{
public:
	FMockCallRecorder Recorder;

	FNavBuildResult BuildNavigationResult;
	FNavBuildResult RebuildNavigationResult;
	FNavFindPathResult FindPathResult;
	FNavProjectPointResult ProjectPointToNavResult;
	FNavRandomPointResult GetRandomReachablePointResult;

	virtual FNavBuildResult BuildNavigation() override { Recorder.RecordCall(TEXT("BuildNavigation")); return BuildNavigationResult; }
	virtual FNavBuildResult RebuildNavigation() override { Recorder.RecordCall(TEXT("RebuildNavigation")); return RebuildNavigationResult; }
	virtual FNavFindPathResult FindPath(const FVector& Start, const FVector& End) override { Recorder.RecordCall(TEXT("FindPath")); return FindPathResult; }
	virtual FNavProjectPointResult ProjectPointToNav(const FVector& Point, const FVector& Extent) override { Recorder.RecordCall(TEXT("ProjectPointToNav")); return ProjectPointToNavResult; }
	virtual FNavRandomPointResult GetRandomReachablePoint(const FVector& Origin, float Radius) override { Recorder.RecordCall(TEXT("GetRandomReachablePoint")); return GetRandomReachablePointResult; }
};
