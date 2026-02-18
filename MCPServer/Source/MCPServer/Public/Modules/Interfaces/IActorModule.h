// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

/**
 * Result of an actor spawn operation.
 */
struct FActorSpawnResult
{
	bool bSuccess = false;
	FString ActorName;
	FString ActorLabel;
	FString ActorClass;
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	FString ErrorMessage;
};

/**
 * Result of an actor delete operation.
 */
struct FActorDeleteResult
{
	bool bSuccess = false;
	FString ActorName;
	FString ErrorMessage;
};

/**
 * Result of an actor duplicate operation.
 */
struct FActorDuplicateResult
{
	bool bSuccess = false;
	FString ActorName;
	FString ActorLabel;
	FString ActorClass;
	FVector Location = FVector::ZeroVector;
	FString ErrorMessage;
};

/**
 * Module interface for actor operations in the editor world.
 * Wraps UE5 editor actor subsystem behind a testable interface.
 */
class IActorModule
{
public:
	virtual ~IActorModule() = default;

	/**
	 * Spawn an actor in the editor world.
	 * @param ActorClassPath  Full class path (e.g. "/Script/Engine.StaticMeshActor")
	 * @param Location        World location to spawn at
	 * @param Rotation        World rotation to spawn with
	 * @return                Spawn result with actor info or error
	 */
	virtual FActorSpawnResult SpawnActor(
		const FString& ActorClassPath,
		const FVector& Location,
		const FRotator& Rotation) = 0;

	/**
	 * Delete an actor from the editor world by name/label/path.
	 * @param ActorIdentifier  Actor name, label, or path (e.g. "StaticMeshActor_0" or "PersistentLevel.StaticMeshActor_0")
	 * @return                 Delete result with success status or error
	 */
	virtual FActorDeleteResult DeleteActor(const FString& ActorIdentifier) = 0;

	/**
	 * Duplicate an actor in the editor world.
	 * @param ActorIdentifier  Actor name, label, or path to duplicate
	 * @param Offset           Translation offset for the duplicate
	 * @return                 Duplicate result with new actor info or error
	 */
	virtual FActorDuplicateResult DuplicateActor(
		const FString& ActorIdentifier,
		const FVector& Offset) = 0;
};
