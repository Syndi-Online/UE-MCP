// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FNavBuildResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FNavFindPathResult
{
	bool bSuccess = false;
	bool bPartialPath = false;
	TArray<FVector> PathPoints;
	FString ErrorMessage;
};

struct FNavProjectPointResult
{
	bool bSuccess = false;
	FVector ProjectedLocation = FVector::ZeroVector;
	FString ErrorMessage;
};

struct FNavRandomPointResult
{
	bool bSuccess = false;
	FVector Location = FVector::ZeroVector;
	FString ErrorMessage;
};

/**
 * Module interface for Navigation / AI operations.
 */
class INavigationModule
{
public:
	virtual ~INavigationModule() = default;

	virtual FNavBuildResult BuildNavigation() = 0;
	virtual FNavBuildResult RebuildNavigation() = 0;
	virtual FNavFindPathResult FindPath(const FVector& Start, const FVector& End) = 0;
	virtual FNavProjectPointResult ProjectPointToNav(const FVector& Point, const FVector& Extent) = 0;
	virtual FNavRandomPointResult GetRandomReachablePoint(const FVector& Origin, float Radius) = 0;
};
