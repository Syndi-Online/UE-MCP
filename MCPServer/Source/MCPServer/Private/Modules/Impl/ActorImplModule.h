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

	virtual FActorDuplicateResult DuplicateActor(
		const FString& ActorIdentifier,
		const FVector& Offset) override;

	virtual FGetActorsResult GetActorsInLevel(const FString& ClassFilter = TEXT("")) override;

	virtual FActorSelectionResult SelectActors(const TArray<FString>& ActorIdentifiers, bool bAddToSelection = false) override;

	virtual FActorSelectionResult DeselectAll() override;

	virtual FActorTransformResult GetActorTransform(const FString& ActorIdentifier) override;

	virtual FActorSetTransformResult SetActorTransform(
		const FString& ActorIdentifier,
		const FVector* Location = nullptr,
		const FRotator* Rotation = nullptr,
		const FVector* Scale = nullptr) override;

	virtual FActorPropertyResult GetActorProperty(const FString& ActorIdentifier, const FString& PropertyName) override;

	virtual FActorSetPropertyResult SetActorProperty(const FString& ActorIdentifier, const FString& PropertyName, const FString& PropertyValue) override;

	virtual FActorConvertResult ConvertActor(const FString& ActorIdentifier, const FString& NewClassPath) override;

	virtual FMoveActorsToLevelResult MoveActorsToLevel(const TArray<FString>& ActorIdentifiers, const FString& LevelName) override;

	virtual FGroupActorsResult GroupActors(const TArray<FString>& ActorIdentifiers, const FString& GroupName = TEXT("")) override;

	virtual FSetActorFolderResult SetActorFolder(const FString& ActorIdentifier, const FString& FolderPath) override;

private:
	/** Find actor by name, label, or path */
	AActor* FindActorByIdentifier(const FString& ActorIdentifier);
};
