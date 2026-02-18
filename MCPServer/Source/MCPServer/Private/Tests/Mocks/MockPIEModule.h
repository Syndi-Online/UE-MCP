// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IPIEModule.h"
#include "MockCallRecorder.h"

class FMockPIEModule : public IPIEModule
{
public:
	FMockCallRecorder Recorder;

	FPIEResult PlayInEditorResult;
	FPIEResult StopPIEResult;
	FPIEResult SimulateInEditorResult;
	FPIEStatusResult IsPlayingResult;

	virtual FPIEResult PlayInEditor() override { Recorder.RecordCall(TEXT("PlayInEditor")); return PlayInEditorResult; }
	virtual FPIEResult StopPIE() override { Recorder.RecordCall(TEXT("StopPIE")); return StopPIEResult; }
	virtual FPIEResult SimulateInEditor() override { Recorder.RecordCall(TEXT("SimulateInEditor")); return SimulateInEditorResult; }
	virtual FPIEStatusResult IsPlaying() override { Recorder.RecordCall(TEXT("IsPlaying")); return IsPlayingResult; }
};
