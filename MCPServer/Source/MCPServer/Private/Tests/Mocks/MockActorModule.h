// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IActorModule.h"
#include "MockCallRecorder.h"

class FMockActorModule : public IActorModule
{
public:
	FMockCallRecorder Recorder;

	// Configurable results
	FActorSpawnResult SpawnActorResult;
	FActorDeleteResult DeleteActorResult;
	FActorDuplicateResult DuplicateActorResult;
	FGetActorsResult GetActorsInLevelResult;
	FActorSelectionResult SelectActorsResult;
	FActorSelectionResult DeselectAllResult;
	FActorTransformResult GetActorTransformResult;
	FActorSetTransformResult SetActorTransformResult;
	FActorPropertyResult GetActorPropertyResult;
	FActorSetPropertyResult SetActorPropertyResult;
	FActorConvertResult ConvertActorResult;
	FMoveActorsToLevelResult MoveActorsToLevelResult;
	FGroupActorsResult GroupActorsResult;
	FSetActorFolderResult SetActorFolderResult;

	virtual FActorSpawnResult SpawnActor(const FString& ActorClassPath, const FVector& Location, const FRotator& Rotation) override
	{
		Recorder.RecordCall(TEXT("SpawnActor"));
		return SpawnActorResult;
	}

	virtual FActorDeleteResult DeleteActor(const FString& ActorIdentifier) override
	{
		Recorder.RecordCall(TEXT("DeleteActor"));
		return DeleteActorResult;
	}

	virtual FActorDuplicateResult DuplicateActor(const FString& ActorIdentifier, const FVector& Offset) override
	{
		Recorder.RecordCall(TEXT("DuplicateActor"));
		return DuplicateActorResult;
	}

	virtual FGetActorsResult GetActorsInLevel(const FString& ClassFilter = TEXT("")) override
	{
		Recorder.RecordCall(TEXT("GetActorsInLevel"));
		return GetActorsInLevelResult;
	}

	virtual FActorSelectionResult SelectActors(const TArray<FString>& ActorIdentifiers, bool bAddToSelection = false) override
	{
		Recorder.RecordCall(TEXT("SelectActors"));
		return SelectActorsResult;
	}

	virtual FActorSelectionResult DeselectAll() override
	{
		Recorder.RecordCall(TEXT("DeselectAll"));
		return DeselectAllResult;
	}

	virtual FActorTransformResult GetActorTransform(const FString& ActorIdentifier) override
	{
		Recorder.RecordCall(TEXT("GetActorTransform"));
		return GetActorTransformResult;
	}

	virtual FActorSetTransformResult SetActorTransform(const FString& ActorIdentifier, const FVector* Location, const FRotator* Rotation, const FVector* Scale) override
	{
		Recorder.RecordCall(TEXT("SetActorTransform"));
		return SetActorTransformResult;
	}

	virtual FActorPropertyResult GetActorProperty(const FString& ActorIdentifier, const FString& PropertyName) override
	{
		Recorder.RecordCall(TEXT("GetActorProperty"));
		return GetActorPropertyResult;
	}

	virtual FActorSetPropertyResult SetActorProperty(const FString& ActorIdentifier, const FString& PropertyName, const FString& PropertyValue) override
	{
		Recorder.RecordCall(TEXT("SetActorProperty"));
		return SetActorPropertyResult;
	}

	virtual FActorConvertResult ConvertActor(const FString& ActorIdentifier, const FString& NewClassPath) override
	{
		Recorder.RecordCall(TEXT("ConvertActor"));
		return ConvertActorResult;
	}

	virtual FMoveActorsToLevelResult MoveActorsToLevel(const TArray<FString>& ActorIdentifiers, const FString& LevelName) override
	{
		Recorder.RecordCall(TEXT("MoveActorsToLevel"));
		return MoveActorsToLevelResult;
	}

	virtual FGroupActorsResult GroupActors(const TArray<FString>& ActorIdentifiers, const FString& GroupName) override
	{
		Recorder.RecordCall(TEXT("GroupActors"));
		return GroupActorsResult;
	}

	virtual FSetActorFolderResult SetActorFolder(const FString& ActorIdentifier, const FString& FolderPath) override
	{
		Recorder.RecordCall(TEXT("SetActorFolder"));
		return SetActorFolderResult;
	}
};
