// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IUMGModule;

class FSetWidgetPropertyImplTool : public IMCPTool
{
public:
	explicit FSetWidgetPropertyImplTool(IUMGModule& InUMGModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IUMGModule& UMGModule;
};
