// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IMaterialModule;

class FGetMaterialExpressionPropertyImplTool : public IMCPTool
{
public:
	explicit FGetMaterialExpressionPropertyImplTool(IMaterialModule& InMaterialModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IMaterialModule& MaterialModule;
};
