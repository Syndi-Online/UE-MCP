// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FEditorBuildResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FBuildStatusResult
{
	bool bSuccess = false;
	bool bIsBuilding = false;
	bool bIsLightingBuilding = false;
	bool bIsNavigationBuilding = false;
	bool bWasCancelled = false;
	FString ErrorMessage;
};

/**
 * Module interface for editor Build operations.
 */
class IBuildModule
{
public:
	virtual ~IBuildModule() = default;

	virtual FEditorBuildResult BuildGeometry() = 0;
	virtual FEditorBuildResult BuildLighting() = 0;
	virtual FEditorBuildResult BuildNavigationData() = 0;
	virtual FEditorBuildResult BuildHLOD() = 0;
	virtual FEditorBuildResult BuildTextureStreaming() = 0;
	virtual FEditorBuildResult BuildAll() = 0;
	virtual FBuildStatusResult GetBuildStatus() = 0;
};
