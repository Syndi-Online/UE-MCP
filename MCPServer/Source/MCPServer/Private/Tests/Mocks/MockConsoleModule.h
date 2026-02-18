// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IConsoleModule.h"
#include "MockCallRecorder.h"

class FMockConsoleModule : public IConsoleModule
{
public:
	FMockCallRecorder Recorder;

	FConsoleCommandResult ExecuteConsoleCommandResult;
	FCVarResult GetCVarResult;
	FCVarResult SetCVarResult;

	virtual FConsoleCommandResult ExecuteConsoleCommand(const FString& Command) override { Recorder.RecordCall(TEXT("ExecuteConsoleCommand")); return ExecuteConsoleCommandResult; }
	virtual FCVarResult GetCVar(const FString& Name) override { Recorder.RecordCall(TEXT("GetCVar")); return GetCVarResult; }
	virtual FCVarResult SetCVar(const FString& Name, const FString& Value) override { Recorder.RecordCall(TEXT("SetCVar")); return SetCVarResult; }
};
