// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IBuildModule.h"

class FBuildImplModule : public IBuildModule
{
public:
	virtual FEditorBuildResult BuildGeometry() override;
	virtual FEditorBuildResult BuildLighting() override;
	virtual FEditorBuildResult BuildNavigationData() override;
	virtual FEditorBuildResult BuildHLOD() override;
	virtual FEditorBuildResult BuildTextureStreaming() override;
	virtual FEditorBuildResult BuildAll() override;
	virtual FBuildStatusResult GetBuildStatus() override;
};
