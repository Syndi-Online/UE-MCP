// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FWorldPartitionResult
{
	bool bSuccess = false;
	FString Message;
	FString ErrorMessage;
};

/**
 * Module interface for World Partition operations.
 */
class IWorldPartitionModule
{
public:
	virtual ~IWorldPartitionModule() = default;

	virtual FWorldPartitionResult BuildHLOD() = 0;
	virtual FWorldPartitionResult DeleteHLOD() = 0;
};
