// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IUIModule;

/**
 * MCP Tool: get_selected_content_browser
 * Get current selection in Content Browser.
 */
class FGetSelectedContentBrowserImplTool : public IMCPTool
{
public:
	explicit FGetSelectedContentBrowserImplTool(IUIModule& InUIModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IUIModule& UIModule;
};
