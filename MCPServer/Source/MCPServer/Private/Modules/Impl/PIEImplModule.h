// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Modules/Interfaces/IPIEModule.h"

class FPIEImplModule : public IPIEModule
{
public:
	virtual FPIEResult PlayInEditor() override;
	virtual FPIEResult StopPIE() override;
	virtual FPIEResult SimulateInEditor() override;
	virtual FPIEStatusResult IsPlaying() override;
};
