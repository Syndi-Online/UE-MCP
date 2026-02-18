// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IPIEModule;

class FIsPlayingImplTool : public IMCPTool
{
public:
	explicit FIsPlayingImplTool(IPIEModule& InPIEModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IPIEModule& PIEModule;
};
