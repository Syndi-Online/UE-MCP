// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FPIEResult
{
	bool bSuccess = false;
	FString Message;
	FString ErrorMessage;
};

struct FPIEStatusResult
{
	bool bSuccess = false;
	bool bIsPlaying = false;
	bool bIsSimulating = false;
	FString ErrorMessage;
};

/**
 * Module interface for Play In Editor operations.
 */
class IPIEModule
{
public:
	virtual ~IPIEModule() = default;

	virtual FPIEResult PlayInEditor() = 0;
	virtual FPIEResult StopPIE() = 0;
	virtual FPIEResult SimulateInEditor() = 0;
	virtual FPIEStatusResult IsPlaying() = 0;
};
