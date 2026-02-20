// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/BlueprintImplModule.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Engine/Blueprint.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompilerModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphNode_Comment.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_Event.h"
#include "K2Node_ComponentBoundEvent.h"
#include "K2Node_VariableGet.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
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

UEdGraph* FBlueprintImplModule::FindGraph(UBlueprint* Blueprint, const FString& GraphName)
{
	TArray<UEdGraph*> AllGraphs;
	Blueprint->GetAllGraphs(AllGraphs);
	for (UEdGraph* Graph : AllGraphs)
	{
		if (Graph && Graph->GetName() == GraphName)
		{
			return Graph;
		}
	}
	return nullptr;
}

UEdGraphNode* FBlueprintImplModule::FindNodeById(UEdGraph* Graph, const FString& NodeId)
{
	for (UEdGraphNode* Node : Graph->Nodes)
	{
		if (Node && Node->NodeGuid.ToString() == NodeId)
		{
			return Node;
		}
	}
	return nullptr;
}

FGetGraphNodesResult FBlueprintImplModule::GetGraphNodes(const FString& BlueprintPath, const FString& GraphName)
{
	FGetGraphNodesResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	for (UEdGraphNode* Node : Graph->Nodes)
	{
		if (!Node) continue;

		FGraphNodeInfo Info;
		Info.NodeId = Node->NodeGuid.ToString();
		Info.NodeClass = Node->GetClass()->GetName();
		Info.NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
		Info.PosX = Node->NodePosX;
		Info.PosY = Node->NodePosY;
		Info.Width = Node->NodeWidth;
		Info.Height = Node->NodeHeight;

		UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(Node);
		if (CommentNode)
		{
			Info.Comment = CommentNode->NodeComment;
		}

		for (UEdGraphPin* Pin : Node->Pins)
		{
			if (!Pin) continue;

			FGraphNodePinInfo PinInfo;
			PinInfo.PinId = Pin->PinId.ToString();
			PinInfo.PinName = Pin->PinName.ToString();
			PinInfo.PinType = Pin->PinType.PinCategory.ToString();
			PinInfo.Direction = (Pin->Direction == EGPD_Input) ? TEXT("Input") : TEXT("Output");

			for (UEdGraphPin* LinkedPin : Pin->LinkedTo)
			{
				if (LinkedPin)
				{
					PinInfo.ConnectedPinIds.Add(LinkedPin->PinId.ToString());
				}
			}

			Info.Pins.Add(PinInfo);
		}

		Result.Nodes.Add(Info);
	}

	Result.bSuccess = true;
	return Result;
}

FSetNodePositionResult FBlueprintImplModule::SetNodePosition(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, int32 PosX, int32 PosY)
{
	FSetNodePositionResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	UEdGraphNode* Node = FindNodeById(Graph, NodeId);
	if (!Node)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Node not found: %s"), *NodeId);
		return Result;
	}

	Node->NodePosX = PosX;
	Node->NodePosY = PosY;
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

FAddCommentBoxResult FBlueprintImplModule::AddCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& CommentText, int32 PosX, int32 PosY, int32 Width, int32 Height, const FLinearColor* Color, const TArray<FString>* NodeIds)
{
	FAddCommentBoxResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	// If node_ids provided, calculate bounding box to wrap them
	int32 FinalPosX = PosX;
	int32 FinalPosY = PosY;
	int32 FinalWidth = Width;
	int32 FinalHeight = Height;

	if (NodeIds && NodeIds->Num() > 0)
	{
		int32 MinX = TNumericLimits<int32>::Max();
		int32 MinY = TNumericLimits<int32>::Max();
		int32 MaxX = TNumericLimits<int32>::Min();
		int32 MaxY = TNumericLimits<int32>::Min();
		int32 FoundCount = 0;

		for (const FString& NId : *NodeIds)
		{
			UEdGraphNode* N = FindNodeById(Graph, NId);
			if (N)
			{
				MinX = FMath::Min(MinX, N->NodePosX);
				MinY = FMath::Min(MinY, N->NodePosY);
				MaxX = FMath::Max(MaxX, N->NodePosX + FMath::Max(N->NodeWidth, 200));
				MaxY = FMath::Max(MaxY, N->NodePosY + FMath::Max(N->NodeHeight, 100));
				FoundCount++;
			}
		}

		if (FoundCount > 0)
		{
			const int32 Padding = 50;
			FinalPosX = MinX - Padding;
			FinalPosY = MinY - Padding - 30; // extra space for comment title
			FinalWidth = (MaxX - MinX) + Padding * 2;
			FinalHeight = (MaxY - MinY) + Padding * 2 + 30;
		}
	}

	UEdGraphNode_Comment* CommentNode = NewObject<UEdGraphNode_Comment>(Graph);
	if (!CommentNode)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Failed to create comment box");
		return Result;
	}

	CommentNode->CreateNewGuid();
	CommentNode->PostPlacedNewNode();
	Graph->AddNode(CommentNode, false, false);

	// Set properties AFTER PostPlacedNewNode() which resets NodeComment to default "Comment"
	CommentNode->NodeComment = CommentText;
	CommentNode->NodePosX = FinalPosX;
	CommentNode->NodePosY = FinalPosY;
	CommentNode->NodeWidth = FinalWidth;
	CommentNode->NodeHeight = FinalHeight;

	if (Color)
	{
		CommentNode->CommentColor = *Color;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.NodeId = CommentNode->NodeGuid.ToString();
	return Result;
}

FDeleteCommentBoxResult FBlueprintImplModule::DeleteCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId)
{
	FDeleteCommentBoxResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	UEdGraphNode* Node = FindNodeById(Graph, NodeId);
	if (!Node)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Node not found: %s"), *NodeId);
		return Result;
	}

	UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(Node);
	if (!CommentNode)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Node %s is not a Comment Box"), *NodeId);
		return Result;
	}

	Graph->RemoveNode(CommentNode);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

FSetCommentBoxPropertiesResult FBlueprintImplModule::SetCommentBoxProperties(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString* CommentText, const FLinearColor* Color, const int32* PosX, const int32* PosY, const int32* Width, const int32* Height)
{
	FSetCommentBoxPropertiesResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	UEdGraphNode* Node = FindNodeById(Graph, NodeId);
	if (!Node)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Node not found: %s"), *NodeId);
		return Result;
	}

	UEdGraphNode_Comment* CommentNode = Cast<UEdGraphNode_Comment>(Node);
	if (!CommentNode)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Node %s is not a Comment Box"), *NodeId);
		return Result;
	}

	if (CommentText)
	{
		CommentNode->NodeComment = *CommentText;
	}
	if (Color)
	{
		CommentNode->CommentColor = *Color;
	}
	if (PosX)
	{
		CommentNode->NodePosX = *PosX;
	}
	if (PosY)
	{
		CommentNode->NodePosY = *PosY;
	}
	if (Width)
	{
		CommentNode->NodeWidth = *Width;
	}
	if (Height)
	{
		CommentNode->NodeHeight = *Height;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

// ============================================================
// Blueprint Components
// ============================================================

FAddBlueprintComponentResult FBlueprintImplModule::AddBlueprintComponent(const FString& BlueprintPath, const FString& ComponentClass, const FString* ComponentName, const FString* ParentComponent)
{
	FAddBlueprintComponentResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (!SCS)
	{
		Result.ErrorMessage = TEXT("Blueprint has no SimpleConstructionScript");
		return Result;
	}

	// Find component class
	FString FullClassName = ComponentClass;
	if (!FullClassName.StartsWith(TEXT("U")))
	{
		FullClassName = TEXT("U") + FullClassName;
	}

	UClass* CompClass = FindFirstObject<UClass>(*FullClassName, EFindFirstObjectOptions::ExactClass);
	if (!CompClass)
	{
		CompClass = FindFirstObject<UClass>(*ComponentClass, EFindFirstObjectOptions::ExactClass);
	}
	if (!CompClass)
	{
		CompClass = LoadObject<UClass>(nullptr, *ComponentClass);
	}
	if (!CompClass || !CompClass->IsChildOf(UActorComponent::StaticClass()))
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component class not found or not a component: %s"), *ComponentClass);
		return Result;
	}

	// Create the SCS node
	USCS_Node* NewNode = SCS->CreateNode(CompClass, ComponentName ? FName(**ComponentName) : NAME_None);
	if (!NewNode)
	{
		Result.ErrorMessage = TEXT("Failed to create SCS node");
		return Result;
	}

	// Attach to parent or root
	if (ParentComponent && !ParentComponent->IsEmpty())
	{
		USCS_Node* ParentNode = nullptr;
		for (USCS_Node* Node : SCS->GetAllNodes())
		{
			if (Node && Node->GetVariableName().ToString() == *ParentComponent)
			{
				ParentNode = Node;
				break;
			}
		}
		if (!ParentNode)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Parent component not found: %s"), **ParentComponent);
			return Result;
		}
		ParentNode->AddChildNode(NewNode);
	}
	else
	{
		SCS->AddNode(NewNode);
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.ComponentName = NewNode->GetVariableName().ToString();
	Result.ComponentClass = CompClass->GetName();
	Result.ParentComponent = ParentComponent ? *ParentComponent : TEXT("(root)");
	return Result;
}

FRemoveBlueprintComponentResult FBlueprintImplModule::RemoveBlueprintComponent(const FString& BlueprintPath, const FString& ComponentName)
{
	FRemoveBlueprintComponentResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (!SCS)
	{
		Result.ErrorMessage = TEXT("Blueprint has no SimpleConstructionScript");
		return Result;
	}

	USCS_Node* TargetNode = nullptr;
	for (USCS_Node* Node : SCS->GetAllNodes())
	{
		if (Node && Node->GetVariableName().ToString() == ComponentName)
		{
			TargetNode = Node;
			break;
		}
	}

	if (!TargetNode)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component not found: %s"), *ComponentName);
		return Result;
	}

	SCS->RemoveNodeAndPromoteChildren(TargetNode);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

FGetBlueprintComponentsResult FBlueprintImplModule::GetBlueprintComponents(const FString& BlueprintPath)
{
	FGetBlueprintComponentsResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (!SCS)
	{
		Result.ErrorMessage = TEXT("Blueprint has no SimpleConstructionScript");
		return Result;
	}

	TArray<USCS_Node*> RootNodes = SCS->GetRootNodes();
	TArray<USCS_Node*> AllNodes = SCS->GetAllNodes();

	for (USCS_Node* Node : AllNodes)
	{
		if (!Node) continue;

		FBlueprintComponentInfo Info;
		Info.ComponentName = Node->GetVariableName().ToString();
		Info.ComponentClass = Node->ComponentClass ? Node->ComponentClass->GetName() : TEXT("Unknown");
		Info.bIsRoot = RootNodes.Contains(Node);

		// Find parent
		for (USCS_Node* PotentialParent : AllNodes)
		{
			if (PotentialParent && PotentialParent->GetChildNodes().Contains(Node))
			{
				Info.ParentComponent = PotentialParent->GetVariableName().ToString();
				break;
			}
		}

		Result.Components.Add(Info);
	}

	Result.bSuccess = true;
	return Result;
}

FSetBlueprintComponentPropertyResult FBlueprintImplModule::SetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName, const FString& PropertyValue)
{
	FSetBlueprintComponentPropertyResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (!SCS)
	{
		Result.ErrorMessage = TEXT("Blueprint has no SimpleConstructionScript");
		return Result;
	}

	USCS_Node* TargetNode = nullptr;
	for (USCS_Node* Node : SCS->GetAllNodes())
	{
		if (Node && Node->GetVariableName().ToString() == ComponentName)
		{
			TargetNode = Node;
			break;
		}
	}

	if (!TargetNode)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component not found: %s"), *ComponentName);
		return Result;
	}

	UActorComponent* ComponentTemplate = TargetNode->ComponentTemplate;
	if (!ComponentTemplate)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component template not found for: %s"), *ComponentName);
		return Result;
	}

	FProperty* Property = ComponentTemplate->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Property not found: %s"), *PropertyName);
		return Result;
	}

	const TCHAR* ImportResult = Property->ImportText_Direct(*PropertyValue, Property->ContainerPtrToValuePtr<void>(ComponentTemplate), ComponentTemplate, PPF_None);
	if (!ImportResult)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Failed to set property '%s' to '%s'"), *PropertyName, *PropertyValue);
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

FGetBlueprintComponentPropertyResult FBlueprintImplModule::GetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName)
{
	FGetBlueprintComponentPropertyResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (!SCS)
	{
		Result.ErrorMessage = TEXT("Blueprint has no SimpleConstructionScript");
		return Result;
	}

	USCS_Node* TargetNode = nullptr;
	for (USCS_Node* Node : SCS->GetAllNodes())
	{
		if (Node && Node->GetVariableName().ToString() == ComponentName)
		{
			TargetNode = Node;
			break;
		}
	}

	if (!TargetNode)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component not found: %s"), *ComponentName);
		return Result;
	}

	UActorComponent* ComponentTemplate = TargetNode->ComponentTemplate;
	if (!ComponentTemplate)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component template not found for: %s"), *ComponentName);
		return Result;
	}

	FProperty* Property = ComponentTemplate->GetClass()->FindPropertyByName(FName(*PropertyName));
	if (!Property)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Property not found: %s"), *PropertyName);
		return Result;
	}

	FString ValueStr;
	Property->ExportTextItem_Direct(ValueStr, Property->ContainerPtrToValuePtr<void>(ComponentTemplate), nullptr, ComponentTemplate, PPF_None);

	Result.bSuccess = true;
	Result.PropertyValue = ValueStr;
	return Result;
}

// ============================================================
// Graph Editing
// ============================================================

FAddGraphNodeResult FBlueprintImplModule::AddGraphNode(const FString& BlueprintPath, const FString& GraphName, const FString& NodeType, const FString* MemberName, const FString* Target, const int32* PosX, const int32* PosY)
{
	FAddGraphNodeResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	const UEdGraphSchema_K2* K2Schema = Cast<UEdGraphSchema_K2>(Graph->GetSchema());
	if (!K2Schema)
	{
		Result.ErrorMessage = TEXT("Graph schema is not K2");
		return Result;
	}

	UEdGraphNode* NewNode = nullptr;

	if (NodeType.Equals(TEXT("CallFunction"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for CallFunction nodes");
			return Result;
		}

		// Find the function
		UClass* TargetClass = nullptr;
		if (Target)
		{
			TargetClass = FindFirstObject<UClass>(**Target, EFindFirstObjectOptions::ExactClass);
			if (!TargetClass)
			{
				FString FullTargetName = TEXT("U") + *Target;
				TargetClass = FindFirstObject<UClass>(*FullTargetName, EFindFirstObjectOptions::ExactClass);
			}
			if (!TargetClass)
			{
				TargetClass = LoadObject<UClass>(nullptr, **Target);
			}
		}

		UFunction* Function = nullptr;
		if (TargetClass)
		{
			Function = TargetClass->FindFunctionByName(FName(**MemberName));
		}
		else
		{
			// Search in common engine libraries
			TArray<UClass*> SearchClasses = {
				UKismetSystemLibrary::StaticClass(),
				UKismetMathLibrary::StaticClass(),
				UKismetStringLibrary::StaticClass(),
				AActor::StaticClass(),
				Blueprint->GeneratedClass
			};
			for (UClass* SearchClass : SearchClasses)
			{
				if (SearchClass)
				{
					Function = SearchClass->FindFunctionByName(FName(**MemberName));
					if (Function)
					{
						TargetClass = SearchClass;
						break;
					}
				}
			}
		}

		if (!Function)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Function not found: %s"), **MemberName);
			return Result;
		}

		UK2Node_CallFunction* CallNode = NewObject<UK2Node_CallFunction>(Graph);
		CallNode->SetFromFunction(Function);
		CallNode->NodePosX = PosX ? *PosX : 0;
		CallNode->NodePosY = PosY ? *PosY : 0;
		CallNode->AllocateDefaultPins();
		Graph->AddNode(CallNode, false, false);
		CallNode->PostPlacedNewNode();
		NewNode = CallNode;
	}
	else if (NodeType.Equals(TEXT("Event"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for Event nodes");
			return Result;
		}

		UK2Node_Event* EventNode = NewObject<UK2Node_Event>(Graph);
		EventNode->EventReference.SetExternalMember(FName(**MemberName), AActor::StaticClass());
		EventNode->bOverrideFunction = true;
		EventNode->NodePosX = PosX ? *PosX : 0;
		EventNode->NodePosY = PosY ? *PosY : 0;
		EventNode->AllocateDefaultPins();
		Graph->AddNode(EventNode, false, false);
		EventNode->PostPlacedNewNode();
		NewNode = EventNode;
	}
	else if (NodeType.Equals(TEXT("VariableGet"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for VariableGet nodes");
			return Result;
		}

		UK2Node_VariableGet* GetNode = NewObject<UK2Node_VariableGet>(Graph);
		GetNode->VariableReference.SetSelfMember(FName(**MemberName));
		GetNode->NodePosX = PosX ? *PosX : 0;
		GetNode->NodePosY = PosY ? *PosY : 0;
		GetNode->AllocateDefaultPins();
		Graph->AddNode(GetNode, false, false);
		GetNode->PostPlacedNewNode();
		NewNode = GetNode;
	}
	else
	{
		Result.ErrorMessage = FString::Printf(TEXT("Unsupported node type: %s. Supported: CallFunction, Event, VariableGet"), *NodeType);
		return Result;
	}

	if (!NewNode)
	{
		Result.ErrorMessage = TEXT("Failed to create node");
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	Result.NodeId = NewNode->NodeGuid.ToString();

	// Collect pin info
	for (UEdGraphPin* Pin : NewNode->Pins)
	{
		if (!Pin) continue;

		FGraphNodePinInfo PinInfo;
		PinInfo.PinId = Pin->PinId.ToString();
		PinInfo.PinName = Pin->PinName.ToString();
		PinInfo.PinType = Pin->PinType.PinCategory.ToString();
		PinInfo.Direction = (Pin->Direction == EGPD_Input) ? TEXT("Input") : TEXT("Output");
		Result.Pins.Add(PinInfo);
	}

	return Result;
}

FConnectGraphPinsResult FBlueprintImplModule::ConnectGraphPins(const FString& BlueprintPath, const FString& GraphName, const FString& SourceNodeId, const FString& SourcePinName, const FString& TargetNodeId, const FString& TargetPinName)
{
	FConnectGraphPinsResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	UEdGraphNode* SourceNode = FindNodeById(Graph, SourceNodeId);
	if (!SourceNode)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Source node not found: %s"), *SourceNodeId);
		return Result;
	}

	UEdGraphNode* TargetNode = FindNodeById(Graph, TargetNodeId);
	if (!TargetNode)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Target node not found: %s"), *TargetNodeId);
		return Result;
	}

	// Find source pin
	UEdGraphPin* SourcePin = nullptr;
	for (UEdGraphPin* Pin : SourceNode->Pins)
	{
		if (Pin && Pin->PinName.ToString() == SourcePinName)
		{
			SourcePin = Pin;
			break;
		}
	}
	if (!SourcePin)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Source pin not found: %s on node %s"), *SourcePinName, *SourceNodeId);
		return Result;
	}

	// Find target pin
	UEdGraphPin* TargetPin = nullptr;
	for (UEdGraphPin* Pin : TargetNode->Pins)
	{
		if (Pin && Pin->PinName.ToString() == TargetPinName)
		{
			TargetPin = Pin;
			break;
		}
	}
	if (!TargetPin)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Target pin not found: %s on node %s"), *TargetPinName, *TargetNodeId);
		return Result;
	}

	const UEdGraphSchema* Schema = Graph->GetSchema();
	if (!Schema->TryCreateConnection(SourcePin, TargetPin))
	{
		Result.ErrorMessage = FString::Printf(TEXT("Cannot connect %s.%s to %s.%s — incompatible types or directions"),
			*SourceNodeId, *SourcePinName, *TargetNodeId, *TargetPinName);
		return Result;
	}

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

FSetPinDefaultValueResult FBlueprintImplModule::SetPinDefaultValue(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString& PinName, const FString& DefaultValue)
{
	FSetPinDefaultValueResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UEdGraph* Graph = FindGraph(Blueprint, GraphName);
	if (!Graph)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Graph not found: %s"), *GraphName);
		return Result;
	}

	UEdGraphNode* Node = FindNodeById(Graph, NodeId);
	if (!Node)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Node not found: %s"), *NodeId);
		return Result;
	}

	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* P : Node->Pins)
	{
		if (P && P->PinName.ToString() == PinName)
		{
			Pin = P;
			break;
		}
	}
	if (!Pin)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Pin not found: %s on node %s"), *PinName, *NodeId);
		return Result;
	}

	if (Pin->Direction != EGPD_Input)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Pin '%s' is not an input pin"), *PinName);
		return Result;
	}

	const UEdGraphSchema* Schema = Graph->GetSchema();
	Schema->TrySetDefaultValue(*Pin, DefaultValue);

	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}
