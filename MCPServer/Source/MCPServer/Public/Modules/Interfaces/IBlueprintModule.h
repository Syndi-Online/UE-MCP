// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

struct FBlueprintCreateResult
{
	bool bSuccess = false;
	FString BlueprintName;
	FString BlueprintPath;
	FString ParentClass;
	FString ErrorMessage;
};

struct FBlueprintFromActorResult
{
	bool bSuccess = false;
	FString BlueprintName;
	FString BlueprintPath;
	FString SourceActorName;
	FString ErrorMessage;
};

struct FBlueprintCompileResult
{
	bool bSuccess = false;
	int32 NumErrors = 0;
	int32 NumWarnings = 0;
	FString ErrorMessage;
};

struct FBlueprintVariableResult
{
	bool bSuccess = false;
	FString VariableName;
	FString ErrorMessage;
};

struct FBlueprintFunctionResult
{
	bool bSuccess = false;
	FString FunctionName;
	FString ErrorMessage;
};

struct FBlueprintInterfaceResult
{
	bool bSuccess = false;
	FString InterfaceName;
	FString ErrorMessage;
};

struct FBlueprintGraphInfo
{
	FString GraphName;
	FString GraphType;
};

struct FBlueprintGraphsResult
{
	bool bSuccess = false;
	TArray<FBlueprintGraphInfo> Graphs;
	FString ErrorMessage;
};

struct FBlueprintReparentResult
{
	bool bSuccess = false;
	FString OldParentClass;
	FString NewParentClass;
	FString ErrorMessage;
};

struct FBlueprintOpenEditorResult
{
	bool bSuccess = false;
	FString BlueprintName;
	FString ErrorMessage;
};

struct FGraphNodePinInfo
{
	FString PinId;
	FString PinName;
	FString PinType;
	FString Direction; // "Input" or "Output"
	TArray<FString> ConnectedPinIds;
};

struct FGraphNodeInfo
{
	FString NodeId;
	FString NodeClass;
	FString NodeTitle;
	int32 PosX = 0;
	int32 PosY = 0;
	int32 Width = 0;
	int32 Height = 0;
	FString Comment;
	TArray<FGraphNodePinInfo> Pins;
};

struct FGetGraphNodesResult
{
	bool bSuccess = false;
	TArray<FGraphNodeInfo> Nodes;
	FString ErrorMessage;
};

struct FSetNodePositionResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FAddCommentBoxResult
{
	bool bSuccess = false;
	FString NodeId;
	FString ErrorMessage;
};

struct FDeleteCommentBoxResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FSetCommentBoxPropertiesResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

// Blueprint Components
struct FBlueprintComponentInfo
{
	FString ComponentName;
	FString ComponentClass;
	FString ParentComponent;
	bool bIsRoot = false;
};

struct FAddBlueprintComponentResult
{
	bool bSuccess = false;
	FString ComponentName;
	FString ComponentClass;
	FString ParentComponent;
	FString ErrorMessage;
};

struct FRemoveBlueprintComponentResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FGetBlueprintComponentsResult
{
	bool bSuccess = false;
	TArray<FBlueprintComponentInfo> Components;
	FString ErrorMessage;
};

struct FSetBlueprintComponentPropertyResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FGetBlueprintComponentPropertyResult
{
	bool bSuccess = false;
	FString PropertyValue;
	FString ErrorMessage;
};

// Graph Editing
struct FAddGraphNodeResult
{
	bool bSuccess = false;
	FString NodeId;
	TArray<FGraphNodePinInfo> Pins;
	FString ErrorMessage;
};

struct FConnectGraphPinsResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

struct FSetPinDefaultValueResult
{
	bool bSuccess = false;
	FString ErrorMessage;
};

/**
 * Module interface for Blueprint operations.
 */
class IBlueprintModule
{
public:
	virtual ~IBlueprintModule() = default;

	virtual FBlueprintCreateResult CreateBlueprint(const FString& BlueprintPath, const FString& ParentClassPath) = 0;
	virtual FBlueprintFromActorResult CreateBlueprintFromActor(const FString& BlueprintPath, const FString& ActorIdentifier) = 0;
	virtual FBlueprintCompileResult CompileBlueprint(const FString& BlueprintPath) = 0;
	virtual FBlueprintVariableResult AddBlueprintVariable(const FString& BlueprintPath, const FString& VariableName, const FString& VariableType) = 0;
	virtual FBlueprintVariableResult RemoveBlueprintVariable(const FString& BlueprintPath, const FString& VariableName) = 0;
	virtual FBlueprintFunctionResult AddBlueprintFunction(const FString& BlueprintPath, const FString& FunctionName) = 0;
	virtual FBlueprintInterfaceResult AddBlueprintInterface(const FString& BlueprintPath, const FString& InterfacePath) = 0;
	virtual FBlueprintGraphsResult GetBlueprintGraphs(const FString& BlueprintPath) = 0;
	virtual FBlueprintReparentResult ReparentBlueprint(const FString& BlueprintPath, const FString& NewParentClassPath) = 0;
	virtual FBlueprintOpenEditorResult OpenBlueprintEditor(const FString& BlueprintPath) = 0;

	virtual FGetGraphNodesResult GetGraphNodes(const FString& BlueprintPath, const FString& GraphName) = 0;
	virtual FSetNodePositionResult SetNodePosition(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, int32 PosX, int32 PosY) = 0;
	virtual FAddCommentBoxResult AddCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& CommentText, int32 PosX, int32 PosY, int32 Width, int32 Height, const FLinearColor* Color, const TArray<FString>* NodeIds) = 0;
	virtual FDeleteCommentBoxResult DeleteCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId) = 0;
	virtual FSetCommentBoxPropertiesResult SetCommentBoxProperties(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString* CommentText, const FLinearColor* Color, const int32* PosX, const int32* PosY, const int32* Width, const int32* Height) = 0;

	// Blueprint Components
	virtual FAddBlueprintComponentResult AddBlueprintComponent(const FString& BlueprintPath, const FString& ComponentClass, const FString* ComponentName, const FString* ParentComponent) = 0;
	virtual FRemoveBlueprintComponentResult RemoveBlueprintComponent(const FString& BlueprintPath, const FString& ComponentName) = 0;
	virtual FGetBlueprintComponentsResult GetBlueprintComponents(const FString& BlueprintPath) = 0;
	virtual FSetBlueprintComponentPropertyResult SetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName, const FString& PropertyValue) = 0;
	virtual FGetBlueprintComponentPropertyResult GetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName) = 0;

	// Graph Editing
	virtual FAddGraphNodeResult AddGraphNode(const FString& BlueprintPath, const FString& GraphName, const FString& NodeType, const FString* MemberName, const FString* Target, const int32* PosX, const int32* PosY) = 0;
	virtual FConnectGraphPinsResult ConnectGraphPins(const FString& BlueprintPath, const FString& GraphName, const FString& SourceNodeId, const FString& SourcePinName, const FString& TargetNodeId, const FString& TargetPinName) = 0;
	virtual FSetPinDefaultValueResult SetPinDefaultValue(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString& PinName, const FString& DefaultValue) = 0;
};
