// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class ISCCModule;

/**
 * MCP Tool: scc_sync
 * Sync files to latest revision from source control.
 */
class FSccSyncImplTool : public IMCPTool
{
public:
	explicit FSccSyncImplTool(ISCCModule& InSCCModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	ISCCModule& SCCModule;
};
