// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/ActorImplModule.h"
#include "Editor.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"

FActorSpawnResult FActorImplModule::SpawnActor(
	const FString& ActorClassPath,
	const FVector& Location,
	const FRotator& Rotation)
{
	FActorSpawnResult Result;

	// Find the class by path
	UClass* ActorClass = FindObject<UClass>(nullptr, *ActorClassPath);
	if (!ActorClass)
	{
		ActorClass = LoadClass<AActor>(nullptr, *ActorClassPath);
	}

	if (!ActorClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Class not found: %s"), *ActorClassPath);
		return Result;
	}

	if (!ActorClass->IsChildOf(AActor::StaticClass()))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Class is not an Actor: %s"), *ActorClassPath);
		return Result;
	}

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	AActor* SpawnedActor = EditorActorSubsystem->SpawnActorFromClass(ActorClass, Location, Rotation);
	if (!SpawnedActor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to spawn actor of class: %s"), *ActorClassPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.ActorName = SpawnedActor->GetName();
	Result.ActorLabel = SpawnedActor->GetActorLabel();
	Result.ActorClass = ActorClass->GetPathName();
	Result.Location = SpawnedActor->GetActorLocation();
	Result.Rotation = SpawnedActor->GetActorRotation();

	return Result;
}

FActorDeleteResult FActorImplModule::DeleteActor(const FString& ActorIdentifier)
{
	FActorDeleteResult Result;

	AActor* FoundActor = FindActorByIdentifier(ActorIdentifier);
	if (!FoundActor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	Result.ActorName = FoundActor->GetActorLabel();
	if (Result.ActorName.IsEmpty())
	{
		Result.ActorName = FoundActor->GetName();
	}

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	bool bDestroyed = EditorActorSubsystem->DestroyActor(FoundActor);
	if (!bDestroyed)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to destroy actor: %s"), *ActorIdentifier);
		return Result;
	}

	Result.bSuccess = true;
	return Result;
}

AActor* FActorImplModule::FindActorByIdentifier(const FString& ActorIdentifier)
{
	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		return nullptr;
	}

	// Try GetActorReference first (supports path like "PersistentLevel.ActorName")
	AActor* FoundActor = EditorActorSubsystem->GetActorReference(ActorIdentifier);
	if (FoundActor)
	{
		return FoundActor;
	}

	// Search by name or label among all level actors
	TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();
	for (AActor* Actor : AllActors)
	{
		if (Actor->GetName() == ActorIdentifier || Actor->GetActorLabel() == ActorIdentifier)
		{
			return Actor;
		}
	}

	return nullptr;
}
