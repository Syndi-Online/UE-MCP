// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/NavigationImplModule.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Editor.h"

FNavBuildResult FNavigationImplModule::BuildNavigation()
{
	FNavBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Navigation system not available");
		return Result;
	}

	NavSys->Build();

	Result.bSuccess = true;
	return Result;
}

FNavBuildResult FNavigationImplModule::RebuildNavigation()
{
	FNavBuildResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Navigation system not available");
		return Result;
	}

	// Mark all navigation data as dirty and rebuild
	NavSys->CancelBuild();
	NavSys->Build();

	Result.bSuccess = true;
	return Result;
}

FNavFindPathResult FNavigationImplModule::FindPath(const FVector& Start, const FVector& End)
{
	FNavFindPathResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(World, Start, End);
	if (!Path || !Path->IsValid())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No path found between the specified points");
		return Result;
	}

	Result.bSuccess = true;
	Result.bPartialPath = Path->IsPartial();
	Result.PathPoints = Path->PathPoints;
	return Result;
}

FNavProjectPointResult FNavigationImplModule::ProjectPointToNav(const FVector& Point, const FVector& Extent)
{
	FNavProjectPointResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Navigation system not available");
		return Result;
	}

	FNavLocation NavLoc;
	bool bProjected = NavSys->ProjectPointToNavigation(Point, NavLoc, Extent);
	if (!bProjected)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to project point to navigation mesh");
		return Result;
	}

	Result.bSuccess = true;
	Result.ProjectedLocation = NavLoc.Location;
	return Result;
}

FNavRandomPointResult FNavigationImplModule::GetRandomReachablePoint(const FVector& Origin, float Radius)
{
	FNavRandomPointResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Navigation system not available");
		return Result;
	}

	FNavLocation RandomLoc;
	bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, Radius, RandomLoc);
	if (!bFound)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to find a random reachable point");
		return Result;
	}

	Result.bSuccess = true;
	Result.Location = RandomLoc.Location;
	return Result;
}
