// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IViewportModule;

class FSetViewModeImplTool : public IMCPTool
{
public:
	explicit FSetViewModeImplTool(IViewportModule& InViewportModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IViewportModule& ViewportModule;
};
