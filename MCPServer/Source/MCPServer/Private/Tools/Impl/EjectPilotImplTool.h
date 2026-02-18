// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IViewportModule;

class FEjectPilotImplTool : public IMCPTool
{
public:
	explicit FEjectPilotImplTool(IViewportModule& InViewportModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IViewportModule& ViewportModule;
};
