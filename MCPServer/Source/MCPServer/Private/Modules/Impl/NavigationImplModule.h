// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/INavigationModule.h"

class FNavigationImplModule : public INavigationModule
{
public:
	virtual FNavBuildResult BuildNavigation() override;
	virtual FNavBuildResult RebuildNavigation() override;
	virtual FNavFindPathResult FindPath(const FVector& Start, const FVector& End) override;
	virtual FNavProjectPointResult ProjectPointToNav(const FVector& Point, const FVector& Extent) override;
	virtual FNavRandomPointResult GetRandomReachablePoint(const FVector& Origin, float Radius) override;
};
