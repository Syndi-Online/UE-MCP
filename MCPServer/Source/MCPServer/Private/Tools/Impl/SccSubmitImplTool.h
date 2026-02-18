// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ISCCModule;

/**
 * MCP Tool: scc_submit
 * Submit checked out files with a description.
 */
class FSccSubmitImplTool : public IMCPTool
{
public:
	explicit FSccSubmitImplTool(ISCCModule& InSCCModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ISCCModule& SCCModule;
};
