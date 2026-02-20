// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IBlueprintModule.h"

class IActorModule;
class UEdGraph;
class UEdGraphNode;

class FBlueprintImplModule : public IBlueprintModule
{
public:
	explicit FBlueprintImplModule(IActorModule& InActorModule);

	virtual FBlueprintCreateResult CreateBlueprint(const FString& BlueprintPath, const FString& ParentClassPath) override;
	virtual FBlueprintFromActorResult CreateBlueprintFromActor(const FString& BlueprintPath, const FString& ActorIdentifier) override;
	virtual FBlueprintCompileResult CompileBlueprint(const FString& BlueprintPath) override;
	virtual FBlueprintVariableResult AddBlueprintVariable(const FString& BlueprintPath, const FString& VariableName, const FString& VariableType) override;
	virtual FBlueprintVariableResult RemoveBlueprintVariable(const FString& BlueprintPath, const FString& VariableName) override;
	virtual FBlueprintFunctionResult AddBlueprintFunction(const FString& BlueprintPath, const FString& FunctionName) override;
	virtual FBlueprintInterfaceResult AddBlueprintInterface(const FString& BlueprintPath, const FString& InterfacePath) override;
	virtual FBlueprintGraphsResult GetBlueprintGraphs(const FString& BlueprintPath) override;
	virtual FBlueprintReparentResult ReparentBlueprint(const FString& BlueprintPath, const FString& NewParentClassPath) override;
	virtual FBlueprintOpenEditorResult OpenBlueprintEditor(const FString& BlueprintPath) override;

	virtual FGetGraphNodesResult GetGraphNodes(const FString& BlueprintPath, const FString& GraphName) override;
	virtual FSetNodePositionResult SetNodePosition(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, int32 PosX, int32 PosY) override;
	virtual FAddCommentBoxResult AddCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& CommentText, int32 PosX, int32 PosY, int32 Width, int32 Height, const FLinearColor* Color, const TArray<FString>* NodeIds) override;
	virtual FDeleteCommentBoxResult DeleteCommentBox(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId) override;
	virtual FSetCommentBoxPropertiesResult SetCommentBoxProperties(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString* CommentText, const FLinearColor* Color, const int32* PosX, const int32* PosY, const int32* Width, const int32* Height) override;

	virtual FAddBlueprintComponentResult AddBlueprintComponent(const FString& BlueprintPath, const FString& ComponentClass, const FString* ComponentName, const FString* ParentComponent) override;
	virtual FRemoveBlueprintComponentResult RemoveBlueprintComponent(const FString& BlueprintPath, const FString& ComponentName) override;
	virtual FGetBlueprintComponentsResult GetBlueprintComponents(const FString& BlueprintPath) override;
	virtual FSetBlueprintComponentPropertyResult SetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName, const FString& PropertyValue) override;
	virtual FGetBlueprintComponentPropertyResult GetBlueprintComponentProperty(const FString& BlueprintPath, const FString& ComponentName, const FString& PropertyName) override;

	virtual FAddGraphNodeResult AddGraphNode(const FString& BlueprintPath, const FString& GraphName, const FString& NodeType, const FString* MemberName, const FString* Target, const int32* PosX, const int32* PosY) override;
	virtual FConnectGraphPinsResult ConnectGraphPins(const FString& BlueprintPath, const FString& GraphName, const FString& SourceNodeId, const FString& SourcePinName, const FString& TargetNodeId, const FString& TargetPinName) override;
	virtual FSetPinDefaultValueResult SetPinDefaultValue(const FString& BlueprintPath, const FString& GraphName, const FString& NodeId, const FString& PinName, const FString& DefaultValue) override;

private:
	UEdGraph* FindGraph(UBlueprint* Blueprint, const FString& GraphName);
	UEdGraphNode* FindNodeById(UEdGraph* Graph, const FString& NodeId);
	IActorModule& ActorModule;
};
