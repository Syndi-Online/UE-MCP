// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ISequencerToolModule;

class FSequencerForceEvaluateImplTool : public IMCPTool
{
public:
	explicit FSequencerForceEvaluateImplTool(ISequencerToolModule& InSequencerModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ISequencerToolModule& SequencerModule;
};
