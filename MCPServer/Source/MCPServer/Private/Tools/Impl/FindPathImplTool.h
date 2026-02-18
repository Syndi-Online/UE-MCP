// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class INavigationModule;

class FFindPathImplTool : public IMCPTool
{
public:
	explicit FFindPathImplTool(INavigationModule& InNavigationModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	INavigationModule& NavigationModule;
};
