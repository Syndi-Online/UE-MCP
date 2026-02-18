// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IUIModule;

/**
 * MCP Tool: show_dialog
 * Show a modal dialog in the editor.
 */
class FShowDialogImplTool : public IMCPTool
{
public:
	explicit FShowDialogImplTool(IUIModule& InUIModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IUIModule& UIModule;
};
