// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/BlueprintImplModule.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Engine/Blueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompilerModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "EdGraph/EdGraph.h"
#include "EdGraphSchema_K2.h"
#include "Editor.h"
#include "GameFramework/Actor.h"

FBlueprintImplModule::FBlueprintImplModule(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FBlueprintCreateResult FBlueprintImplModule::CreateBlueprint(const FString& BlueprintPath, const FString& ParentClassPath)
{
	FBlueprintCreateResult Result;

	// Find parent class
	UClass* ParentClass = LoadObject<UClass>(nullptr, *ParentClassPath);
	if (!ParentClass)
	{
		ParentClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *ParentClassPath));
	}
	if (!ParentClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Parent class not found: %s"), *ParentClassPath);
		return Result;
	}

	if (!FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass))
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Cannot create Blueprint from class: %s"), *ParentClassPath);
		return Result;
	}

	// Extract package path and asset name
	FString PackagePath = FPackageName::GetLongPackagePath(BlueprintPath);
	FString AssetName = FPackageName::GetLongPackageAssetName(BlueprintPath);

	if (AssetName.IsEmpty())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Invalid Blueprint path: could not extract asset name");
		return Result;
	}

	// Get blueprint class types
	UClass* BpClass = nullptr;
	UClass* BpGenClass = nullptr;
	IKismetCompilerInterface& Compiler = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
	Compiler.GetBlueprintTypesForClass(ParentClass, BpClass, BpGenClass);

	// Create package
	UPackage* Package = CreatePackage(*BlueprintPath);
	if (!Package)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create package: %s"), *BlueprintPath);
		return Result;
	}

	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprint(
		ParentClass,
		Package,
		FName(*AssetName),
		BPTYPE_Normal,
		BpClass,
		BpGenClass
	);

	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to create Blueprint");
		return Result;
	}

	FAssetRegistryModule::AssetCreated(Blueprint);
	Blueprint->MarkPackageDirty();

	Result.bSuccess = true;
	Result.BlueprintName = AssetName;
	Result.BlueprintPath = Blueprint->GetPathName();
	Result.ParentClass = ParentClass->GetName();
	return Result;
}

FBlueprintFromActorResult FBlueprintImplModule::CreateBlueprintFromActor(const FString& BlueprintPath, const FString& ActorIdentifier)
{
	FBlueprintFromActorResult Result;

	UEditorActorSubsystem* EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	if (!EditorActorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UEditorActorSubsystem is not available");
		return Result;
	}

	// Find the actor
	AActor* Actor = nullptr;
	TArray<AActor*> AllActors = EditorActorSubsystem->GetAllLevelActors();
	for (AActor* A : AllActors)
	{
		if (A && (A->GetName() == ActorIdentifier || A->GetActorLabel() == ActorIdentifier))
		{
			Actor = A;
			break;
		}
	}

	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorIdentifier);
		return Result;
	}

	FKismetEditorUtilities::FCreateBlueprintFromActorParams Params;
	Params.bOpenBlueprint = false;

	UBlueprint* Blueprint = FKismetEditorUtilities::CreateBlueprintFromActor(BlueprintPath, Actor, Params);

	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to create Blueprint from actor");
		return Result;
	}

	Result.bSuccess = true;
	Result.BlueprintName = Blueprint->GetName();
	Result.BlueprintPath = Blueprint->GetPathName();
	Result.SourceActorName = Actor->GetActorLabel();
	return Result;
}

FBlueprintCompileResult FBlueprintImplModule::CompileBlueprint(const FString& BlueprintPath)
{
	FBlueprintCompileResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::None, nullptr);

	Result.bSuccess = (Blueprint->Status != BS_Error);
	Result.NumErrors = (Blueprint->Status == BS_Error) ? 1 : 0;
	Result.NumWarnings = 0;
	if (!Result.bSuccess)
	{
		Result.ErrorMessage = TEXT("Blueprint compilation failed with errors");
	}
	return Result;
}

FBlueprintVariableResult FBlueprintImplModule::AddBlueprintVariable(const FString& BlueprintPath, const FString& VariableName, const FString& VariableType)
{
	FBlueprintVariableResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	// Map type string to pin type
	FEdGraphPinType PinType;
	if (VariableType.Equals(TEXT("Boolean"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("bool"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
	}
	else if (VariableType.Equals(TEXT("Integer"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("int"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("int32"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Int;
	}
	else if (VariableType.Equals(TEXT("Int64"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("Integer64"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Int64;
	}
	else if (VariableType.Equals(TEXT("Float"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("Double"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("Real"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Real;
		PinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
	}
	else if (VariableType.Equals(TEXT("String"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("FString"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_String;
	}
	else if (VariableType.Equals(TEXT("Name"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("FName"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Name;
	}
	else if (VariableType.Equals(TEXT("Text"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("FText"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Text;
	}
	else if (VariableType.Equals(TEXT("Vector"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("FVector"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		PinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();
	}
	else if (VariableType.Equals(TEXT("Rotator"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("FRotator"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		PinType.PinSubCategoryObject = TBaseStructure<FRotator>::Get();
	}
	else if (VariableType.Equals(TEXT("Transform"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("FTransform"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
		PinType.PinSubCategoryObject = TBaseStructure<FTransform>::Get();
	}
	else if (VariableType.Equals(TEXT("Byte"), ESearchCase::IgnoreCase) || VariableType.Equals(TEXT("uint8"), ESearchCase::IgnoreCase))
	{
		PinType.PinCategory = UEdGraphSchema_K2::PC_Byte;
	}
	else
	{
		// Try to find it as a class for object references
		UClass* VarClass = LoadObject<UClass>(nullptr, *VariableType);
		if (!VarClass)
		{
			VarClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *VariableType));
		}
		if (VarClass)
		{
			PinType.PinCategory = UEdGraphSchema_K2::PC_Object;
			PinType.PinSubCategoryObject = VarClass;
		}
		else
		{
			Result.bSuccess = false;
			Result.ErrorMessage = FString::Printf(TEXT("Unknown variable type: %s. Supported: Boolean, Integer, Int64, Float, String, Name, Text, Vector, Rotator, Transform, Byte, or a class path"), *VariableType);
			return Result;
		}
	}

	bool bAdded = FBlueprintEditorUtils::AddMemberVariable(Blueprint, FName(*VariableName), PinType, TEXT(""));
	if (!bAdded)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add variable '%s' to Blueprint"), *VariableName);
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.VariableName = VariableName;
	return Result;
}

FBlueprintVariableResult FBlueprintImplModule::RemoveBlueprintVariable(const FString& BlueprintPath, const FString& VariableName)
{
	FBlueprintVariableResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	FBlueprintEditorUtils::RemoveMemberVariable(Blueprint, FName(*VariableName));
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.VariableName = VariableName;
	return Result;
}

FBlueprintFunctionResult FBlueprintImplModule::AddBlueprintFunction(const FString& BlueprintPath, const FString& FunctionName)
{
	FBlueprintFunctionResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(
		Blueprint,
		FName(*FunctionName),
		UEdGraph::StaticClass(),
		UEdGraphSchema_K2::StaticClass()
	);

	if (!NewGraph)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create function graph '%s'"), *FunctionName);
		return Result;
	}

	FBlueprintEditorUtils::AddFunctionGraph(Blueprint, NewGraph, true, static_cast<UFunction*>(nullptr));
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.FunctionName = FunctionName;
	return Result;
}

FBlueprintInterfaceResult FBlueprintImplModule::AddBlueprintInterface(const FString& BlueprintPath, const FString& InterfacePath)
{
	FBlueprintInterfaceResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	// Parse the interface path into package and class name
	// Expected format: "/Script/ModuleName.ClassName" or just "ClassName"
	FString PackageName;
	FString ClassName;
	if (InterfacePath.Split(TEXT("."), &PackageName, &ClassName))
	{
		// Full path like "/Script/Engine.ActorComponent"
	}
	else
	{
		// Short name - try as-is
		ClassName = InterfacePath;
		PackageName = TEXT("");
	}

	bool bAdded = FBlueprintEditorUtils::ImplementNewInterface(Blueprint, FTopLevelAssetPath(FName(*PackageName), FName(*ClassName)));
	if (!bAdded)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add interface: %s"), *InterfacePath);
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.InterfaceName = InterfacePath;
	return Result;
}

FBlueprintGraphsResult FBlueprintImplModule::GetBlueprintGraphs(const FString& BlueprintPath)
{
	FBlueprintGraphsResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	// Event graphs
	for (UEdGraph* Graph : Blueprint->UbergraphPages)
	{
		if (Graph)
		{
			FBlueprintGraphInfo Info;
			Info.GraphName = Graph->GetName();
			Info.GraphType = TEXT("EventGraph");
			Result.Graphs.Add(Info);
		}
	}

	// Function graphs
	for (UEdGraph* Graph : Blueprint->FunctionGraphs)
	{
		if (Graph)
		{
			FBlueprintGraphInfo Info;
			Info.GraphName = Graph->GetName();
			Info.GraphType = TEXT("Function");
			Result.Graphs.Add(Info);
		}
	}

	// Macro graphs
	for (UEdGraph* Graph : Blueprint->MacroGraphs)
	{
		if (Graph)
		{
			FBlueprintGraphInfo Info;
			Info.GraphName = Graph->GetName();
			Info.GraphType = TEXT("Macro");
			Result.Graphs.Add(Info);
		}
	}

	// Delegate graphs
	for (UEdGraph* Graph : Blueprint->DelegateSignatureGraphs)
	{
		if (Graph)
		{
			FBlueprintGraphInfo Info;
			Info.GraphName = Graph->GetName();
			Info.GraphType = TEXT("Delegate");
			Result.Graphs.Add(Info);
		}
	}

	Result.bSuccess = true;
	return Result;
}

FBlueprintReparentResult FBlueprintImplModule::ReparentBlueprint(const FString& BlueprintPath, const FString& NewParentClassPath)
{
	FBlueprintReparentResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UClass* NewParentClass = LoadObject<UClass>(nullptr, *NewParentClassPath);
	if (!NewParentClass)
	{
		NewParentClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *NewParentClassPath));
	}
	if (!NewParentClass)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("New parent class not found: %s"), *NewParentClassPath);
		return Result;
	}

	Result.OldParentClass = Blueprint->ParentClass ? Blueprint->ParentClass->GetName() : TEXT("None");

	Blueprint->ParentClass = NewParentClass;
	FBlueprintEditorUtils::RefreshAllNodes(Blueprint);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	FKismetEditorUtilities::CompileBlueprint(Blueprint, EBlueprintCompileOptions::SkipSave, nullptr);

	Result.bSuccess = true;
	Result.NewParentClass = NewParentClass->GetName();
	return Result;
}

FBlueprintOpenEditorResult FBlueprintImplModule::OpenBlueprintEditor(const FString& BlueprintPath)
{
	FBlueprintOpenEditorResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	if (!AssetEditorSubsystem)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("UAssetEditorSubsystem is not available");
		return Result;
	}

	bool bOpened = AssetEditorSubsystem->OpenEditorForAsset(Blueprint);
	if (!bOpened)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Failed to open editor for Blueprint: %s"), *BlueprintPath);
		return Result;
	}

	Result.bSuccess = true;
	Result.BlueprintName = Blueprint->GetName();
	return Result;
}
