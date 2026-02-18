// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tools/Interfaces/IMCPTool.h"

class IUIModule;

/**
 * MCP Tool: show_notification
 * Show a toast notification in the editor.
 */
class FShowNotificationImplTool : public IMCPTool
{
public:
	explicit FShowNotificationImplTool(IUIModule& InUIModule);

	virtual FString GetName() const override;
	virtual FString GetDescription() const override;
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override;
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override;

private:
	IUIModule& UIModule;
};
