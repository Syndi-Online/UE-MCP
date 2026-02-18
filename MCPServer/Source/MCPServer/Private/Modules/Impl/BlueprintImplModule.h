// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IBlueprintModule.h"

class IActorModule;

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

private:
	IActorModule& ActorModule;
};
