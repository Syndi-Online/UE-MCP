// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IBuildModule.h"
#include "MockCallRecorder.h"

class FMockBuildModule : public IBuildModule
{
public:
	FMockCallRecorder Recorder;

	FEditorBuildResult BuildGeometryResult;
	FEditorBuildResult BuildLightingResult;
	FEditorBuildResult BuildNavigationDataResult;
	FEditorBuildResult BuildHLODResult;
	FEditorBuildResult BuildTextureStreamingResult;
	FEditorBuildResult BuildAllResult;
	FBuildStatusResult GetBuildStatusResult;

	virtual FEditorBuildResult BuildGeometry() override { Recorder.RecordCall(TEXT("BuildGeometry")); return BuildGeometryResult; }
	virtual FEditorBuildResult BuildLighting() override { Recorder.RecordCall(TEXT("BuildLighting")); return BuildLightingResult; }
	virtual FEditorBuildResult BuildNavigationData() override { Recorder.RecordCall(TEXT("BuildNavigationData")); return BuildNavigationDataResult; }
	virtual FEditorBuildResult BuildHLOD() override { Recorder.RecordCall(TEXT("BuildHLOD")); return BuildHLODResult; }
	virtual FEditorBuildResult BuildTextureStreaming() override { Recorder.RecordCall(TEXT("BuildTextureStreaming")); return BuildTextureStreamingResult; }
	virtual FEditorBuildResult BuildAll() override { Recorder.RecordCall(TEXT("BuildAll")); return BuildAllResult; }
	virtual FBuildStatusResult GetBuildStatus() override { Recorder.RecordCall(TEXT("GetBuildStatus")); return GetBuildStatusResult; }
};
