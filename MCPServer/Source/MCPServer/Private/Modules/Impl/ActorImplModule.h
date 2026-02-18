// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IActorModule.h"

/**
 * Actor module implementation using UEditorActorSubsystem.
 */
class FActorImplModule : public IActorModule
{
public:
	virtual FActorSpawnResult SpawnActor(
		const FString& ActorClassPath,
		const FVector& Location,
		const FRotator& Rotation) override;

	virtual FActorDeleteResult DeleteActor(const FString& ActorIdentifier) override;

private:
	/** Find actor by name, label, or path */
	AActor* FindActorByIdentifier(const FString& ActorIdentifier);
};
