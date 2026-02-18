// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IWorldPartitionModule.h"
#include "MockCallRecorder.h"

class FMockWorldPartitionModule : public IWorldPartitionModule
{
public:
	FMockCallRecorder Recorder;

	FWorldPartitionResult BuildHLODResult;
	FWorldPartitionResult DeleteHLODResult;

	virtual FWorldPartitionResult BuildHLOD() override { Recorder.RecordCall(TEXT("BuildHLOD")); return BuildHLODResult; }
	virtual FWorldPartitionResult DeleteHLOD() override { Recorder.RecordCall(TEXT("DeleteHLOD")); return DeleteHLODResult; }
};
