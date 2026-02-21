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
#include "K2Node_VariableSet.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_MacroInstance.h"
#include "K2Node_SwitchEnum.h"
#include "K2Node_MapForEach.h"
#include "K2Node_FormatText.h"
#include "K2Node_GetEnumeratorNameAsString.h"
#include "K2Node_EditablePinBase.h"
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

	// Use MarkBlueprintAsStructurallyModified to trigger skeleton class regeneration
	// so that new component variables are immediately available for VariableGet nodes
	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

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

	// Search in this BP's SCS first
	UActorComponent* ComponentTemplate = nullptr;
	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (SCS)
	{
		for (USCS_Node* Node : SCS->GetAllNodes())
		{
			if (Node && Node->GetVariableName().ToString() == ComponentName)
			{
				ComponentTemplate = Node->ComponentTemplate;
				break;
			}
		}
	}

	// If not found, search up the inheritance hierarchy via CDO
	if (!ComponentTemplate)
	{
		UClass* GenClass = Blueprint->GeneratedClass;
		if (GenClass)
		{
			AActor* CDO = Cast<AActor>(GenClass->GetDefaultObject());
			if (CDO)
			{
				TInlineComponentArray<UActorComponent*> Components;
				CDO->GetComponents(Components);
				for (UActorComponent* Comp : Components)
				{
					if (Comp && Comp->GetName() == ComponentName)
					{
						ComponentTemplate = Comp;
						break;
					}
				}
			}
		}
	}

	if (!ComponentTemplate)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Component not found: %s"), *ComponentName);
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
		CallNode->CreateNewGuid();
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
		EventNode->CreateNewGuid();
		EventNode->EventReference.SetExternalMember(FName(**MemberName), AActor::StaticClass());
		EventNode->bOverrideFunction = true;
		EventNode->NodePosX = PosX ? *PosX : 0;
		EventNode->NodePosY = PosY ? *PosY : 0;
		EventNode->AllocateDefaultPins();
		Graph->AddNode(EventNode, false, false);
		EventNode->PostPlacedNewNode();
		NewNode = EventNode;
	}
	else if (NodeType.Equals(TEXT("ComponentBoundEvent"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for ComponentBoundEvent nodes (delegate name, e.g. OnComponentBeginOverlap)");
			return Result;
		}
		if (!Target)
		{
			Result.ErrorMessage = TEXT("target is required for ComponentBoundEvent nodes (component variable name, e.g. CollisionComp)");
			return Result;
		}

		// Find component in SCS
		USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
		if (!SCS)
		{
			Result.ErrorMessage = TEXT("Blueprint has no SimpleConstructionScript");
			return Result;
		}

		USCS_Node* CompNode = nullptr;
		for (USCS_Node* Node : SCS->GetAllNodes())
		{
			if (Node && Node->GetVariableName().ToString() == *Target)
			{
				CompNode = Node;
				break;
			}
		}
		if (!CompNode || !CompNode->ComponentClass)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Component variable not found in SCS: %s"), **Target);
			return Result;
		}

		// Find the FObjectProperty on the blueprint's skeleton class
		UClass* SkeletonClass = Blueprint->SkeletonGeneratedClass;
		if (!SkeletonClass)
		{
			SkeletonClass = Blueprint->GeneratedClass;
		}
		FObjectProperty* ComponentProperty = nullptr;
		if (SkeletonClass)
		{
			ComponentProperty = FindFProperty<FObjectProperty>(SkeletonClass, CompNode->GetVariableName());
		}
		if (!ComponentProperty)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Component property not found on blueprint class: %s"), **Target);
			return Result;
		}

		// Find the multicast delegate property on the component class
		FMulticastDelegateProperty* DelegateProperty = FindFProperty<FMulticastDelegateProperty>(CompNode->ComponentClass, FName(**MemberName));
		if (!DelegateProperty)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Delegate not found on component class %s: %s"), *CompNode->ComponentClass->GetName(), **MemberName);
			return Result;
		}

		UK2Node_ComponentBoundEvent* BoundEventNode = NewObject<UK2Node_ComponentBoundEvent>(Graph);
		BoundEventNode->CreateNewGuid();
		BoundEventNode->InitializeComponentBoundEventParams(ComponentProperty, DelegateProperty);
		BoundEventNode->NodePosX = PosX ? *PosX : 0;
		BoundEventNode->NodePosY = PosY ? *PosY : 0;
		BoundEventNode->AllocateDefaultPins();
		Graph->AddNode(BoundEventNode, false, false);
		BoundEventNode->ReconstructNode();
		NewNode = BoundEventNode;
	}
	else if (NodeType.Equals(TEXT("VariableGet"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for VariableGet nodes");
			return Result;
		}

		UK2Node_VariableGet* GetNode = NewObject<UK2Node_VariableGet>(Graph);
		GetNode->CreateNewGuid();

		if (Target)
		{
			UClass* MemberClass = FindFirstObject<UClass>(**Target, EFindFirstObjectOptions::ExactClass);
			if (!MemberClass)
			{
				FString WithU = TEXT("U") + *Target;
				MemberClass = FindFirstObject<UClass>(*WithU, EFindFirstObjectOptions::ExactClass);
			}
			if (!MemberClass)
			{
				FString WithA = TEXT("A") + *Target;
				MemberClass = FindFirstObject<UClass>(*WithA, EFindFirstObjectOptions::ExactClass);
			}
			if (!MemberClass)
			{
				MemberClass = LoadObject<UClass>(nullptr, **Target);
			}
			if (!MemberClass)
			{
				Result.ErrorMessage = FString::Printf(TEXT("Target class not found: %s"), **Target);
				return Result;
			}
			GetNode->VariableReference.SetExternalMember(FName(**MemberName), MemberClass);
		}
		else
		{
			GetNode->VariableReference.SetSelfMember(FName(**MemberName));
		}

		GetNode->NodePosX = PosX ? *PosX : 0;
		GetNode->NodePosY = PosY ? *PosY : 0;
		GetNode->AllocateDefaultPins();
		Graph->AddNode(GetNode, false, false);
		GetNode->PostPlacedNewNode();
		NewNode = GetNode;
	}
	else if (NodeType.Equals(TEXT("VariableSet"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for VariableSet nodes");
			return Result;
		}

		UK2Node_VariableSet* SetNode = NewObject<UK2Node_VariableSet>(Graph);
		SetNode->CreateNewGuid();
		SetNode->VariableReference.SetSelfMember(FName(**MemberName));
		SetNode->NodePosX = PosX ? *PosX : 0;
		SetNode->NodePosY = PosY ? *PosY : 0;
		SetNode->AllocateDefaultPins();
		Graph->AddNode(SetNode, false, false);
		SetNode->PostPlacedNewNode();
		NewNode = SetNode;
	}
	else if (NodeType.Equals(TEXT("DynamicCast"), ESearchCase::IgnoreCase))
	{
		if (!Target)
		{
			Result.ErrorMessage = TEXT("target is required for DynamicCast nodes (class to cast to)");
			return Result;
		}

		UClass* CastClass = FindFirstObject<UClass>(**Target, EFindFirstObjectOptions::ExactClass);
		if (!CastClass)
		{
			CastClass = LoadObject<UClass>(nullptr, **Target);
		}
		if (!CastClass)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Cast target class not found: %s"), **Target);
			return Result;
		}

		UK2Node_DynamicCast* CastNode = NewObject<UK2Node_DynamicCast>(Graph);
		CastNode->CreateNewGuid();
		CastNode->TargetType = CastClass;
		CastNode->NodePosX = PosX ? *PosX : 0;
		CastNode->NodePosY = PosY ? *PosY : 0;
		CastNode->AllocateDefaultPins();
		Graph->AddNode(CastNode, false, false);
		CastNode->PostPlacedNewNode();
		NewNode = CastNode;
	}
	else if (NodeType.Equals(TEXT("IfThenElse"), ESearchCase::IgnoreCase) || NodeType.Equals(TEXT("Branch"), ESearchCase::IgnoreCase))
	{
		UK2Node_IfThenElse* BranchNode = NewObject<UK2Node_IfThenElse>(Graph);
		BranchNode->CreateNewGuid();
		BranchNode->NodePosX = PosX ? *PosX : 0;
		BranchNode->NodePosY = PosY ? *PosY : 0;
		BranchNode->AllocateDefaultPins();
		Graph->AddNode(BranchNode, false, false);
		BranchNode->PostPlacedNewNode();
		NewNode = BranchNode;
	}
	else if (NodeType.Equals(TEXT("MacroInstance"), ESearchCase::IgnoreCase))
	{
		if (!MemberName)
		{
			Result.ErrorMessage = TEXT("member_name is required for MacroInstance nodes (macro graph name, e.g. ForEachLoop)");
			return Result;
		}

		// Search for the macro graph in the blueprint itself, then in engine macros
		UEdGraph* MacroGraph = nullptr;

		// 1. Search in this blueprint's macro graphs
		for (UEdGraph* G : Blueprint->MacroGraphs)
		{
			if (G && G->GetName() == *MemberName)
			{
				MacroGraph = G;
				break;
			}
		}

		// 2. Search in parent class blueprints
		if (!MacroGraph && Blueprint->ParentClass)
		{
			UBlueprint* ParentBP = Cast<UBlueprint>(Blueprint->ParentClass->ClassGeneratedBy);
			while (ParentBP && !MacroGraph)
			{
				for (UEdGraph* G : ParentBP->MacroGraphs)
				{
					if (G && G->GetName() == *MemberName)
					{
						MacroGraph = G;
						break;
					}
				}
				ParentBP = ParentBP->ParentClass ? Cast<UBlueprint>(ParentBP->ParentClass->ClassGeneratedBy) : nullptr;
			}
		}

		// 3. Search in standard macro libraries (e.g. StandardMacros, which contains ForEachLoop, etc.)
		if (!MacroGraph)
		{
			TArray<UBlueprint*> MacroLibraries;
			for (TObjectIterator<UBlueprint> It; It; ++It)
			{
				UBlueprint* BP = *It;
				if (BP && BP->BlueprintType == BPTYPE_MacroLibrary)
				{
					for (UEdGraph* G : BP->MacroGraphs)
					{
						if (G && G->GetName() == *MemberName)
						{
							MacroGraph = G;
							break;
						}
					}
					if (MacroGraph) break;
				}
			}
		}

		if (!MacroGraph)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Macro graph not found: %s"), **MemberName);
			return Result;
		}

		UK2Node_MacroInstance* MacroNode = NewObject<UK2Node_MacroInstance>(Graph);
		MacroNode->CreateNewGuid();
		MacroNode->SetMacroGraph(MacroGraph);
		MacroNode->NodePosX = PosX ? *PosX : 0;
		MacroNode->NodePosY = PosY ? *PosY : 0;
		MacroNode->AllocateDefaultPins();
		Graph->AddNode(MacroNode, false, false);
		MacroNode->PostPlacedNewNode();
		NewNode = MacroNode;
	}
	else if (NodeType.Equals(TEXT("SwitchEnum"), ESearchCase::IgnoreCase))
	{
		if (!Target)
		{
			Result.ErrorMessage = TEXT("target is required for SwitchEnum nodes (enum type path, e.g. /Script/Engine.ECollisionChannel)");
			return Result;
		}

		UEnum* EnumType = FindFirstObject<UEnum>(**Target, EFindFirstObjectOptions::ExactClass);
		if (!EnumType)
		{
			EnumType = LoadObject<UEnum>(nullptr, **Target);
		}
		if (!EnumType)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Enum type not found: %s"), **Target);
			return Result;
		}

		UK2Node_SwitchEnum* SwitchNode = NewObject<UK2Node_SwitchEnum>(Graph);
		SwitchNode->CreateNewGuid();
		SwitchNode->SetEnum(EnumType);
		SwitchNode->NodePosX = PosX ? *PosX : 0;
		SwitchNode->NodePosY = PosY ? *PosY : 0;
		SwitchNode->AllocateDefaultPins();
		Graph->AddNode(SwitchNode, false, false);
		SwitchNode->PostPlacedNewNode();
		NewNode = SwitchNode;
	}
	else if (NodeType.Equals(TEXT("MapForEach"), ESearchCase::IgnoreCase))
	{
		UK2Node_MapForEach* MapForEachNode = NewObject<UK2Node_MapForEach>(Graph);
		MapForEachNode->CreateNewGuid();
		MapForEachNode->NodePosX = PosX ? *PosX : 0;
		MapForEachNode->NodePosY = PosY ? *PosY : 0;
		MapForEachNode->AllocateDefaultPins();
		Graph->AddNode(MapForEachNode, false, false);
		MapForEachNode->PostPlacedNewNode();
		NewNode = MapForEachNode;
	}
	else if (NodeType.Equals(TEXT("FormatText"), ESearchCase::IgnoreCase))
	{
		UK2Node_FormatText* FormatTextNode = NewObject<UK2Node_FormatText>(Graph);
		FormatTextNode->CreateNewGuid();
		FormatTextNode->NodePosX = PosX ? *PosX : 0;
		FormatTextNode->NodePosY = PosY ? *PosY : 0;
		FormatTextNode->AllocateDefaultPins();
		Graph->AddNode(FormatTextNode, false, false);
		FormatTextNode->PostPlacedNewNode();

		// If member_name is provided, use it as the format string
		if (MemberName)
		{
			UEdGraphPin* FormatPin = FormatTextNode->GetFormatPin();
			if (FormatPin)
			{
				const UEdGraphSchema* Schema = Graph->GetSchema();
				Schema->TrySetDefaultValue(*FormatPin, *MemberName);
			}
		}

		NewNode = FormatTextNode;
	}
	else if (NodeType.Equals(TEXT("GetEnumeratorNameAsString"), ESearchCase::IgnoreCase) || NodeType.Equals(TEXT("EnumToString"), ESearchCase::IgnoreCase))
	{
		UK2Node_GetEnumeratorNameAsString* EnumToStringNode = NewObject<UK2Node_GetEnumeratorNameAsString>(Graph);
		EnumToStringNode->CreateNewGuid();
		EnumToStringNode->NodePosX = PosX ? *PosX : 0;
		EnumToStringNode->NodePosY = PosY ? *PosY : 0;
		EnumToStringNode->AllocateDefaultPins();
		Graph->AddNode(EnumToStringNode, false, false);
		EnumToStringNode->PostPlacedNewNode();
		NewNode = EnumToStringNode;
	}
	else
	{
		Result.ErrorMessage = FString::Printf(TEXT("Unsupported node type: %s. Supported: CallFunction, Event, ComponentBoundEvent, VariableGet, VariableSet, DynamicCast, IfThenElse/Branch, MacroInstance, SwitchEnum, MapForEach, FormatText, EnumToString/GetEnumeratorNameAsString"), *NodeType);
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

FDeleteGraphNodeResult FBlueprintImplModule::DeleteGraphNode(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId)
{
	FDeleteGraphNodeResult Result;

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

	Graph->RemoveNode(Node);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

// ============================================================
// Event Dispatchers
// ============================================================

FAddEventDispatcherResult FBlueprintImplModule::AddEventDispatcher(const FString& BlueprintPath, const FString& DispatcherName, const TArray<FEventDispatcherParamInfo>* Parameters)
{
	FAddEventDispatcherResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	// Step 1: Create the MC delegate member variable
	FEdGraphPinType DelegatePinType;
	DelegatePinType.PinCategory = UEdGraphSchema_K2::PC_MCDelegate;

	bool bAdded = FBlueprintEditorUtils::AddMemberVariable(Blueprint, FName(*DispatcherName), DelegatePinType, TEXT(""));
	if (!bAdded)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Failed to add event dispatcher '%s' to Blueprint"), *DispatcherName);
		return Result;
	}

	// Step 2: Create the delegate signature graph (same as BlueprintEditor.cpp pattern)
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraph* NewGraph = FBlueprintEditorUtils::CreateNewGraph(Blueprint, FName(*DispatcherName), UEdGraph::StaticClass(), UEdGraphSchema_K2::StaticClass());
	if (!NewGraph)
	{
		FBlueprintEditorUtils::RemoveMemberVariable(Blueprint, FName(*DispatcherName));
		Result.ErrorMessage = FString::Printf(TEXT("Failed to create delegate signature graph for '%s'"), *DispatcherName);
		return Result;
	}

	NewGraph->bEditable = false;
	K2Schema->CreateDefaultNodesForGraph(*NewGraph);
	K2Schema->CreateFunctionGraphTerminators(*NewGraph, static_cast<UClass*>(nullptr));
	K2Schema->AddExtraFunctionFlags(NewGraph, FUNC_BlueprintCallable | FUNC_BlueprintEvent | FUNC_Public);
	K2Schema->MarkFunctionEntryAsEditable(NewGraph, true);

	Blueprint->DelegateSignatureGraphs.Add(NewGraph);

	// Step 3: Add parameters to the delegate signature entry node
	if (Parameters && Parameters->Num() > 0)
	{
		// Find the function entry node
		UK2Node_EditablePinBase* EntryNode = nullptr;
		for (UEdGraphNode* Node : NewGraph->Nodes)
		{
			EntryNode = Cast<UK2Node_EditablePinBase>(Node);
			if (EntryNode)
			{
				break;
			}
		}

		if (EntryNode)
		{
			for (const FEventDispatcherParamInfo& Param : *Parameters)
			{
				FEdGraphPinType ParamPinType;
				const FString& ParamType = Param.ParamType;

				if (ParamType.Equals(TEXT("Boolean"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("bool"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
				}
				else if (ParamType.Equals(TEXT("Integer"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("int"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("int32"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Int;
				}
				else if (ParamType.Equals(TEXT("Int64"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("Integer64"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Int64;
				}
				else if (ParamType.Equals(TEXT("Float"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("Double"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("Real"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Real;
					ParamPinType.PinSubCategory = UEdGraphSchema_K2::PC_Double;
				}
				else if (ParamType.Equals(TEXT("String"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("FString"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_String;
				}
				else if (ParamType.Equals(TEXT("Name"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("FName"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Name;
				}
				else if (ParamType.Equals(TEXT("Text"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("FText"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Text;
				}
				else if (ParamType.Equals(TEXT("Vector"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("FVector"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
					ParamPinType.PinSubCategoryObject = TBaseStructure<FVector>::Get();
				}
				else if (ParamType.Equals(TEXT("Rotator"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("FRotator"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
					ParamPinType.PinSubCategoryObject = TBaseStructure<FRotator>::Get();
				}
				else if (ParamType.Equals(TEXT("Transform"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("FTransform"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Struct;
					ParamPinType.PinSubCategoryObject = TBaseStructure<FTransform>::Get();
				}
				else if (ParamType.Equals(TEXT("Byte"), ESearchCase::IgnoreCase) || ParamType.Equals(TEXT("uint8"), ESearchCase::IgnoreCase))
				{
					ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Byte;
				}
				else
				{
					UClass* VarClass = LoadObject<UClass>(nullptr, *ParamType);
					if (!VarClass)
					{
						VarClass = Cast<UClass>(StaticFindFirstObject(UClass::StaticClass(), *ParamType));
					}
					if (VarClass)
					{
						ParamPinType.PinCategory = UEdGraphSchema_K2::PC_Object;
						ParamPinType.PinSubCategoryObject = VarClass;
					}
					else
					{
						Result.ErrorMessage = FString::Printf(TEXT("Unknown parameter type: %s for parameter '%s'"), *ParamType, *Param.ParamName);
						return Result;
					}
				}

				EntryNode->CreateUserDefinedPin(FName(*Param.ParamName), ParamPinType, EGPD_Output);
			}
		}
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);

	Result.bSuccess = true;
	Result.DispatcherName = DispatcherName;
	Result.GraphName = NewGraph->GetName();
	return Result;
}

// ============================================================
// Blueprint Parent Class
// ============================================================

FGetBlueprintParentClassResult FBlueprintImplModule::GetBlueprintParentClass(const FString& BlueprintPath)
{
	FGetBlueprintParentClassResult Result;

	UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);
	if (!Blueprint)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Blueprint not found: %s"), *BlueprintPath);
		return Result;
	}

	UClass* ParentClass = Blueprint->ParentClass;
	if (!ParentClass)
	{
		Result.ErrorMessage = TEXT("No parent class");
		return Result;
	}

	Result.bSuccess = true;
	Result.ParentClass = ParentClass->GetPathName();

	// Check if parent is a Blueprint-generated class
	UBlueprint* ParentBP = Cast<UBlueprint>(ParentClass->ClassGeneratedBy);
	if (ParentBP)
	{
		Result.ParentBlueprint = ParentBP->GetPathName();
	}
	// else ParentBlueprint stays empty (it's a C++ class)

	return Result;
}

// ============================================================
// Batch Graph Node Creation
// ============================================================

FAddGraphNodesBatchResult FBlueprintImplModule::AddGraphNodesBatch(const FString& BlueprintPath, const FString& GraphName, const TArray<FAddGraphNodesBatchNodeInfo>& Nodes, const TArray<FAddGraphNodesBatchConnection>* Connections)
{
	FAddGraphNodesBatchResult Result;

	// Create each node using AddGraphNode
	TMap<FString, FString> LocalIdToNodeId; // local_id -> actual node GUID

	for (const FAddGraphNodesBatchNodeInfo& NodeInfo : Nodes)
	{
		const FString* MemberNamePtr = NodeInfo.MemberName.IsEmpty() ? nullptr : &NodeInfo.MemberName;
		const FString* TargetPtr = NodeInfo.Target.IsEmpty() ? nullptr : &NodeInfo.Target;
		int32 PosX = NodeInfo.PosX;
		int32 PosY = NodeInfo.PosY;

		FAddGraphNodeResult NodeResult = AddGraphNode(BlueprintPath, GraphName, NodeInfo.NodeType, MemberNamePtr, TargetPtr, &PosX, &PosY);
		if (!NodeResult.bSuccess)
		{
			Result.ErrorMessage = FString::Printf(TEXT("Failed to create node '%s': %s"), *NodeInfo.LocalId, *NodeResult.ErrorMessage);
			return Result;
		}

		LocalIdToNodeId.Add(NodeInfo.LocalId, NodeResult.NodeId);

		FAddGraphNodesBatchResultNode ResNode;
		ResNode.LocalId = NodeInfo.LocalId;
		ResNode.NodeId = NodeResult.NodeId;
		ResNode.Pins = NodeResult.Pins;
		Result.Nodes.Add(ResNode);
	}

	// Connect pins if provided
	if (Connections)
	{
		for (const FAddGraphNodesBatchConnection& Conn : *Connections)
		{
			FString* SourceNodeId = LocalIdToNodeId.Find(Conn.SourceLocalId);
			FString* TargetNodeId = LocalIdToNodeId.Find(Conn.TargetLocalId);

			if (!SourceNodeId) { Result.ErrorMessage = FString::Printf(TEXT("Source local_id not found: %s"), *Conn.SourceLocalId); return Result; }
			if (!TargetNodeId) { Result.ErrorMessage = FString::Printf(TEXT("Target local_id not found: %s"), *Conn.TargetLocalId); return Result; }

			FConnectGraphPinsResult ConnResult = ConnectGraphPins(BlueprintPath, GraphName, *SourceNodeId, Conn.SourcePinName, *TargetNodeId, Conn.TargetPinName);
			if (!ConnResult.bSuccess)
			{
				Result.ErrorMessage = FString::Printf(TEXT("Failed to connect %s.%s -> %s.%s: %s"), *Conn.SourceLocalId, *Conn.SourcePinName, *Conn.TargetLocalId, *Conn.TargetPinName, *ConnResult.ErrorMessage);
				return Result;
			}
			Result.ConnectionsMade++;
		}
	}

	Result.bSuccess = true;
	return Result;
}

// ============================================================
// Disconnect Graph Pins
// ============================================================

FDisconnectGraphPinsResult FBlueprintImplModule::DisconnectGraphPins(const FString& BlueprintPath, const FString& GraphName, const FString& SourceNodeId, const FString& SourcePinName, const FString& TargetNodeId, const FString& TargetPinName)
{
	FDisconnectGraphPinsResult Result;

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

	// Check if the pins are actually connected
	if (!SourcePin->LinkedTo.Contains(TargetPin))
	{
		Result.ErrorMessage = FString::Printf(TEXT("Pins are not connected: %s.%s and %s.%s"),
			*SourceNodeId, *SourcePinName, *TargetNodeId, *TargetPinName);
		return Result;
	}

	SourcePin->BreakLinkTo(TargetPin);
	FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);

	Result.bSuccess = true;
	return Result;
}

// ============================================================
// Spatial Queries
// ============================================================

FGetGraphNodesInAreaResult FBlueprintImplModule::GetGraphNodesInArea(const FString& BlueprintPath, const FString& GraphName, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY)
{
	FGetGraphNodesInAreaResult Result;

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

	for (UEdGraphNode* Node : Graph->Nodes)
	{
		if (!Node) continue;

		int32 NodeW = FMath::Max(Node->NodeWidth, 200);
		int32 NodeH = FMath::Max(Node->NodeHeight, 100);

		// AABB intersection test
		bool bIntersects = Node->NodePosX < MaxX && (Node->NodePosX + NodeW) > MinX
			&& Node->NodePosY < MaxY && (Node->NodePosY + NodeH) > MinY;

		if (bIntersects)
		{
			FGraphNodeInAreaInfo Info;
			Info.NodeId = Node->NodeGuid.ToString();
			Info.NodeTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
			Info.PosX = Node->NodePosX;
			Info.PosY = Node->NodePosY;
			Info.Width = NodeW;
			Info.Height = NodeH;
			Result.Nodes.Add(Info);
		}
	}

	Result.bSuccess = true;
	return Result;
}
