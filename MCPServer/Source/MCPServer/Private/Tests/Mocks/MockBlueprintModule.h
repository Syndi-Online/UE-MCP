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
};
