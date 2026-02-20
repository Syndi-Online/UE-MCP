// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IBlueprintModule.h"
#include "MockCallRecorder.h"

class FMockBlueprintModule : public IBlueprintModule
{
public:
	FMockCallRecorder Recorder;

	FBlueprintCreateResult CreateBlueprintResult;
	FBlueprintFromActorResult CreateBlueprintFromActorResult;
	FBlueprintCompileResult CompileBlueprintResult;
	FBlueprintVariableResult AddBlueprintVariableResult;
	FBlueprintVariableResult RemoveBlueprintVariableResult;
	FBlueprintFunctionResult AddBlueprintFunctionResult;
	FBlueprintInterfaceResult AddBlueprintInterfaceResult;
	FBlueprintGraphsResult GetBlueprintGraphsResult;
	FBlueprintReparentResult ReparentBlueprintResult;
	FBlueprintOpenEditorResult OpenBlueprintEditorResult;

	virtual FBlueprintCreateResult CreateBlueprint(const FString& BlueprintPath, const FString& ParentClassPath) override { Recorder.RecordCall(TEXT("CreateBlueprint")); return CreateBlueprintResult; }
	virtual FBlueprintFromActorResult CreateBlueprintFromActor(const FString& BlueprintPath, const FString& ActorIdentifier) override { Recorder.RecordCall(TEXT("CreateBlueprintFromActor")); return CreateBlueprintFromActorResult; }
	virtual FBlueprintCompileResult CompileBlueprint(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("CompileBlueprint")); return CompileBlueprintResult; }
	virtual FBlueprintVariableResult AddBlueprintVariable(const FString& BlueprintPath, const FString& VariableName, const FString& VariableType) override { Recorder.RecordCall(TEXT("AddBlueprintVariable")); return AddBlueprintVariableResult; }
	virtual FBlueprintVariableResult RemoveBlueprintVariable(const FString& BlueprintPath, const FString& VariableName) override { Recorder.RecordCall(TEXT("RemoveBlueprintVariable")); return RemoveBlueprintVariableResult; }
	virtual FBlueprintFunctionResult AddBlueprintFunction(const FString& BlueprintPath, const FString& FunctionName) override { Recorder.RecordCall(TEXT("AddBlueprintFunction")); return AddBlueprintFunctionResult; }
	virtual FBlueprintInterfaceResult AddBlueprintInterface(const FString& BlueprintPath, const FString& InterfacePath) override { Recorder.RecordCall(TEXT("AddBlueprintInterface")); return AddBlueprintInterfaceResult; }
	virtual FBlueprintGraphsResult GetBlueprintGraphs(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("GetBlueprintGraphs")); return GetBlueprintGraphsResult; }
	virtual FBlueprintReparentResult ReparentBlueprint(const FString& BlueprintPath, const FString& NewParentClassPath) override { Recorder.RecordCall(TEXT("ReparentBlueprint")); return ReparentBlueprintResult; }
	virtual FBlueprintOpenEditorResult OpenBlueprintEditor(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("OpenBlueprintEditor")); return OpenBlueprintEditorResult; }

	FGetGraphNodesResult GetGraphNodesResult;
	FSetNodePositionResult SetNodePositionResult;
	FAddCommentBoxResult AddCommentBoxResult;
	FString LastAddCommentBoxText;
	FDeleteCommentBoxResult DeleteCommentBoxResult;
	FSetCommentBoxPropertiesResult SetCommentBoxPropertiesResult;

	virtual FGetGraphNodesResult GetGraphNodes(const FString& BlueprintPath, const FString& GraphName) override { Recorder.RecordCall(TEXT("GetGraphNodes")); return GetGraphNodesResult; }
	virtual FSetNodePositionResult SetNodePosition(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, int32 PosX, int32 PosY) override { Recorder.RecordCall(TEXT("SetNodePosition")); return SetNodePositionResult; }
	virtual FAddCommentBoxResult AddCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& CommentText, int32 PosX, int32 PosY, int32 Width, int32 Height, const FLinearColor* Color, const TArray<FString>* NodeIds) override { Recorder.RecordCall(TEXT("AddCommentBox")); LastAddCommentBoxText = CommentText; return AddCommentBoxResult; }
	virtual FDeleteCommentBoxResult DeleteCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId) override { Recorder.RecordCall(TEXT("DeleteCommentBox")); return DeleteCommentBoxResult; }
	virtual FSetCommentBoxPropertiesResult SetCommentBoxProperties(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString* CommentText, const FLinearColor* Color, const int32* PosX, const int32* PosY, const int32* Width, const int32* Height) override { Recorder.RecordCall(TEXT("SetCommentBoxProperties")); return SetCommentBoxPropertiesResult; }

	// Blueprint Components
	FAddBlueprintComponentResult AddBlueprintComponentResult;
	FRemoveBlueprintComponentResult RemoveBlueprintComponentResult;
	FGetBlueprintComponentsResult GetBlueprintComponentsResult;
	FSetBlueprintComponentPropertyResult SetBlueprintComponentPropertyResult;
	FGetBlueprintComponentPropertyResult GetBlueprintComponentPropertyResult;

	virtual FAddBlueprintComponentResult AddBlueprintComponent(const FString& BlueprintPath, const FString& ComponentClass, const FString* ComponentName, const FString* ParentComponent) override { Recorder.RecordCall(TEXT("AddBlueprintComponent")); return AddBlueprintComponentResult; }
	virtual FRemoveBlueprintComponentResult RemoveBlueprintComponent(const FString& BlueprintPath, const FString& ComponentName) override { Recorder.RecordCall(TEXT("RemoveBlueprintComponent")); return RemoveBlueprintComponentResult; }
	virtual FGetBlueprintComponentsResult GetBlueprintComponents(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("GetBlueprintComponents")); return GetBlueprintComponentsResult; }
	virtual FSetBlueprintComponentPropertyResult SetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName, const FString& PropertyValue) override { Recorder.RecordCall(TEXT("SetBlueprintComponentProperty")); return SetBlueprintComponentPropertyResult; }
	virtual FGetBlueprintComponentPropertyResult GetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName) override { Recorder.RecordCall(TEXT("GetBlueprintComponentProperty")); return GetBlueprintComponentPropertyResult; }

	// Graph Editing
	FAddGraphNodeResult AddGraphNodeResult;
	FConnectGraphPinsResult ConnectGraphPinsResult;
	FSetPinDefaultValueResult SetPinDefaultValueResult;

	virtual FAddGraphNodeResult AddGraphNode(const FString& BlueprintPath, const FString& GraphName, const FString& NodeType, const FString* MemberName, const FString* Target, const int32* PosX, const int32* PosY) override { Recorder.RecordCall(TEXT("AddGraphNode")); return AddGraphNodeResult; }
	virtual FConnectGraphPinsResult ConnectGraphPins(const FString& BlueprintPath, const FString& GraphName, const FString& SourceNodeId, const FString& SourcePinName, const FString& TargetNodeId, const FString& TargetPinName) override { Recorder.RecordCall(TEXT("ConnectGraphPins")); return ConnectGraphPinsResult; }
	virtual FSetPinDefaultValueResult SetPinDefaultValue(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString& PinName, const FString& DefaultValue) override { Recorder.RecordCall(TEXT("SetPinDefaultValue")); return SetPinDefaultValueResult; }

	FDeleteGraphNodeResult DeleteGraphNodeResult;

	virtual FDeleteGraphNodeResult DeleteGraphNode(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId) override { Recorder.RecordCall(TEXT("DeleteGraphNode")); return DeleteGraphNodeResult; }

	// Event Dispatchers
	FAddEventDispatcherResult AddEventDispatcherResult;

	virtual FAddEventDispatcherResult AddEventDispatcher(const FString& BlueprintPath, const FString& DispatcherName, const TArray<FEventDispatcherParamInfo>* Parameters) override { Recorder.RecordCall(TEXT("AddEventDispatcher")); return AddEventDispatcherResult; }

	// Parent class
	FGetBlueprintParentClassResult GetBlueprintParentClassResult;

	virtual FGetBlueprintParentClassResult GetBlueprintParentClass(const FString& BlueprintPath) override { Recorder.RecordCall(TEXT("GetBlueprintParentClass")); return GetBlueprintParentClassResult; }

	// Batch graph operations
	FAddGraphNodesBatchResult AddGraphNodesBatchResult;

	virtual FAddGraphNodesBatchResult AddGraphNodesBatch(const FString& BlueprintPath, const FString& GraphName, const TArray<FAddGraphNodesBatchNodeInfo>& Nodes, const TArray<FAddGraphNodesBatchConnection>* Connections) override { Recorder.RecordCall(TEXT("AddGraphNodesBatch")); return AddGraphNodesBatchResult; }

	// Spatial queries
	FGetGraphNodesInAreaResult GetGraphNodesInAreaResult;

	virtual FGetGraphNodesInAreaResult GetGraphNodesInArea(const FString& BlueprintPath, const FString& GraphName, int32 MinX, int32 MinY, int32 MaxX, int32 MaxY) override { Recorder.RecordCall(TEXT("GetGraphNodesInArea")); return GetGraphNodesInAreaResult; }
};
