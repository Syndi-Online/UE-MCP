// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IUIModule;

/**
 * MCP Tool: sync_content_browser
 * Navigate Content Browser to a path.
 */
class FSyncContentBrowserImplTool : public IMCPTool
{
public:
	explicit FSyncContentBrowserImplTool(IUIModule& InUIModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IUIModule& UIModule;
};
