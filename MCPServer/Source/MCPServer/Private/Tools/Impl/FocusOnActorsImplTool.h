// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IViewportModule;

class FFocusOnActorsImplTool : public IMCPTool
{
public:
	explicit FFocusOnActorsImplTool(IViewportModule& InViewportModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IViewportModule& ViewportModule;
};
