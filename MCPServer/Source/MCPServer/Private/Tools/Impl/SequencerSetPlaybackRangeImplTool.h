// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ISequencerToolModule;

class FSequencerSetPlaybackRangeImplTool : public IMCPTool
{
public:
	explicit FSequencerSetPlaybackRangeImplTool(ISequencerToolModule& InSequencerModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ISequencerToolModule& SequencerModule;
};
