// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/ActorImplModule.h"
#include "Editor.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/TextProperty.h"
#include "Engine/Level.h"
#include "Engine/LevelStreaming.h"
#include "ActorGroupingUtils.h"
#include "Editor/GroupActor.h"

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

FActorDuplicateResult FActorImplModule::DuplicateActor(
	const FString& ActorIdentifier,
	const FVector& Offset)
{
	FActorDuplicateResult Result;

	AActor* SourceActor = FindActorByIdentifier(ActorIdentifier);
	if (!SourceActor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	AActor* DuplicatedActor = EditorActorSubsystem->DuplicateActor(SourceActor, nullptr, Offset);
	if (!DuplicatedActor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to duplicate actor: %s"), *ActorIdentifier);
		return Result;
	}

	Result.bSuccess = true;
	Result.ActorName = DuplicatedActor->GetName();
	Result.ActorLabel = DuplicatedActor->GetActorLabel();
	Result.ActorClass = DuplicatedActor->GetClass()->GetPathName();
	Result.Location = DuplicatedActor->GetActorLocation();

	return Result;
}

FGetActorsResult FActorImplModule::GetActorsInLevel(const FString& ClassFilter)
{
	FGetActorsResult Result;

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();

	UClass* FilterClass = nullptr;
	if (!ClassFilter.IsEmpty())
	{
		FilterClass = FindObject<UClass>(nullptr, *ClassFilter);
		if (!FilterClass)
		{
			FilterClass = LoadClass<AActor>(nullptr, *ClassFilter);
		}
	}

	for (AActor* Actor : AllActors)
	{
		if (!Actor) continue;

		if (FilterClass && !Actor->GetClass()->IsChildOf(FilterClass))
		{
			continue;
		}

		FActorInfo Info;
		Info.ActorName = Actor->GetName();
		Info.ActorLabel = Actor->GetActorLabel();
		Info.ActorClass = Actor->GetClass()->GetPathName();
		Info.Location = Actor->GetActorLocation();
		Info.Rotation = Actor->GetActorRotation();
		Info.FolderPath = Actor->GetFolderPath().ToString();
		Result.Actors.Add(Info);
	}

	Result.bSuccess = true;
	return Result;
}

FActorSelectionResult FActorImplModule::SelectActors(const TArray<FString>& ActorIdentifiers, bool bAddToSelection)
{
	FActorSelectionResult Result;

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	if (!bAddToSelection)
	{
		GEditor->SelectNone(true, true, false);
	}

	int32 SelectedCount = 0;
	for (const FString& Identifier : ActorIdentifiers)
	{
		AActor* Actor = FindActorByIdentifier(Identifier);
		if (Actor)
		{
			GEditor->SelectActor(Actor, true, true, false);
			SelectedCount++;
		}
	}

	Result.bSuccess = true;
	Result.AffectedCount = SelectedCount;
	return Result;
}

FActorSelectionResult FActorImplModule::DeselectAll()
{
	FActorSelectionResult Result;

	GEditor->SelectNone(true, true, false);

	Result.bSuccess = true;
	Result.AffectedCount = 0;
	return Result;
}

FActorTransformResult FActorImplModule::GetActorTransform(const FString& ActorIdentifier)
{
	FActorTransformResult Result;

	AActor* Actor = FindActorByIdentifier(ActorIdentifier);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	FTransform Transform = Actor->GetActorTransform();
	Result.bSuccess = true;
	Result.Location = Transform.GetLocation();
	Result.Rotation = Transform.Rotator();
	Result.Scale = Transform.GetScale3D();
	return Result;
}

FActorSetTransformResult FActorImplModule::SetActorTransform(
	const FString& ActorIdentifier,
	const FVector* Location,
	const FRotator* Rotation,
	const FVector* Scale)
{
	FActorSetTransformResult Result;

	AActor* Actor = FindActorByIdentifier(ActorIdentifier);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	if (Location)
	{
		Actor->SetActorLocation(*Location);
	}
	if (Rotation)
	{
		Actor->SetActorRotation(*Rotation);
	}
	if (Scale)
	{
		Actor->SetActorScale3D(*Scale);
	}

	FTransform NewTransform = Actor->GetActorTransform();
	Result.bSuccess = true;
	Result.Location = NewTransform.GetLocation();
	Result.Rotation = NewTransform.Rotator();
	Result.Scale = NewTransform.GetScale3D();
	return Result;
}

FActorPropertyResult FActorImplModule::GetActorProperty(const FString& ActorIdentifier, const FString& PropertyName)
{
	FActorPropertyResult Result;

	AActor* Actor = FindActorByIdentifier(ActorIdentifier);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	FProperty* Property = Actor->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Property not found: %s on actor %s"), *PropertyName, *ActorIdentifier);
		return Result;
	}

	FString ValueStr;
	const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Actor);
	Property->ExportTextItem_Direct(ValueStr, ValuePtr, nullptr, Actor, PPF_None);

	Result.bSuccess = true;
	Result.PropertyName = PropertyName;
	Result.PropertyValue = ValueStr;
	Result.PropertyType = Property->GetCPPType();
	return Result;
}

FActorSetPropertyResult FActorImplModule::SetActorProperty(const FString& ActorIdentifier, const FString& PropertyName, const FString& PropertyValue)
{
	FActorSetPropertyResult Result;

	AActor* Actor = FindActorByIdentifier(ActorIdentifier);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	FProperty* Property = Actor->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Property not found: %s on actor %s"), *PropertyName, *ActorIdentifier);
		return Result;
	}

	Actor->Modify();

	void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Actor);
	const TCHAR* ImportResult = Property->ImportText_Direct(*PropertyValue, ValuePtr, Actor, PPF_None);

	if (!ImportResult)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to set property %s to value: %s"), *PropertyName, *PropertyValue);
		return Result;
	}

	Actor->PostEditChange();

	Result.bSuccess = true;
	return Result;
}

FActorConvertResult FActorImplModule::ConvertActor(const FString& ActorIdentifier, const FString& NewClassPath)
{
	FActorConvertResult Result;

	AActor* Actor = FindActorByIdentifier(ActorIdentifier);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	UClass* NewClass = FindObject<UClass>(nullptr, *NewClassPath);
	if (!NewClass)
	{
		NewClass = LoadClass<AActor>(nullptr, *NewClassPath);
	}
	if (!NewClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("New class not found: %s"), *NewClassPath);
		return Result;
	}

	if (!NewClass->IsChildOf(AActor::StaticClass()))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Class is not an Actor: %s"), *NewClassPath);
		return Result;
	}

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	// Capture the old transform
	FTransform OldTransform = Actor->GetActorTransform();
	FString OldLabel = Actor->GetActorLabel();

	// Destroy old actor and spawn new one
	EditorActorSubsystem->DestroyActor(Actor);

	AActor* NewActor = EditorActorSubsystem->SpawnActorFromClass(NewClass, OldTransform.GetLocation(), OldTransform.Rotator());
	if (!NewActor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to spawn replacement actor of class: %s"), *NewClassPath);
		return Result;
	}

	NewActor->SetActorScale3D(OldTransform.GetScale3D());

	Result.bSuccess = true;
	Result.NewActorName = NewActor->GetName();
	Result.NewActorLabel = NewActor->GetActorLabel();
	Result.NewActorClass = NewClass->GetPathName();
	return Result;
}

FMoveActorsToLevelResult FActorImplModule::MoveActorsToLevel(const TArray<FString>& ActorIdentifiers, const FString& LevelName)
{
	FMoveActorsToLevelResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	// Find target level
	ULevel* TargetLevel = nullptr;
	for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
	{
		if (StreamingLevel && StreamingLevel->GetWorldAssetPackageFName().ToString().Contains(LevelName))
		{
			TargetLevel = StreamingLevel->GetLoadedLevel();
			break;
		}
	}

	// Check persistent level
	if (!TargetLevel && World->PersistentLevel)
	{
		FString PersistentName = World->PersistentLevel->GetOutermost()->GetName();
		if (PersistentName.Contains(LevelName) || LevelName.Equals(TEXT("PersistentLevel"), ESearchCase::IgnoreCase))
		{
			TargetLevel = World->PersistentLevel;
		}
	}

	if (!TargetLevel)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Level not found: %s"), *LevelName);
		return Result;
	}

	// Find and move actors
	TArray<AActor*> ActorsToMove;
	for (const FString& Identifier : ActorIdentifiers)
	{
		AActor* Actor = FindActorByIdentifier(Identifier);
		if (Actor)
		{
			ActorsToMove.Add(Actor);
		}
	}

	if (ActorsToMove.Num() == 0)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No valid actors found to move");
		return Result;
	}

	int32 MovedCount = 0;
	for (AActor* Actor : ActorsToMove)
	{
		if (Actor->GetLevel() != TargetLevel)
		{
			// Change the actor's outer to the target level
			bool bMoved = Actor->Rename(nullptr, TargetLevel);
			if (bMoved)
			{
				MovedCount++;
			}
		}
		else
		{
			MovedCount++; // Already in target level
		}
	}

	Result.bSuccess = true;
	Result.MovedCount = MovedCount;
	return Result;
}

FGroupActorsResult FActorImplModule::GroupActors(const TArray<FString>& ActorIdentifiers, const FString& GroupName)
{
	FGroupActorsResult Result;

	TArray<AActor*> ActorsToGroup;
	for (const FString& Identifier : ActorIdentifiers)
	{
		AActor* Actor = FindActorByIdentifier(Identifier);
		if (Actor)
		{
			ActorsToGroup.Add(Actor);
		}
	}

	if (ActorsToGroup.Num() < 2)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Need at least 2 actors to create a group");
		return Result;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	// Select the actors first, then use UActorGroupingUtils
	GEditor->SelectNone(true, true, false);
	for (AActor* Actor : ActorsToGroup)
	{
		GEditor->SelectActor(Actor, true, true, false);
	}

	UActorGroupingUtils::Get()->GroupSelected();

	Result.bSuccess = true;
	Result.GroupName = GroupName.IsEmpty() ? TEXT("NewGroup") : GroupName;
	Result.ActorCount = ActorsToGroup.Num();
	return Result;
}

FSetActorFolderResult FActorImplModule::SetActorFolder(const FString& ActorIdentifier, const FString& FolderPath)
{
	FSetActorFolderResult Result;

	AActor* Actor = FindActorByIdentifier(ActorIdentifier);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	Actor->SetFolderPath(FName(*FolderPath));

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
