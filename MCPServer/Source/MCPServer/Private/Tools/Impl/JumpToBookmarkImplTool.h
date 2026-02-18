// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IViewportModule;

class FJumpToBookmarkImplTool : public IMCPTool
{
public:
	explicit FJumpToBookmarkImplTool(IViewportModule& InViewportModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IViewportModule& ViewportModule;
};
