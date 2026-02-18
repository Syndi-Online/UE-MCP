// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IWorldPartitionModule.h"

class FWorldPartitionImplModule : public IWorldPartitionModule
{
public:
	virtual FWorldPartitionResult BuildHLOD() override;
	virtual FWorldPartitionResult DeleteHLOD() override;
};
