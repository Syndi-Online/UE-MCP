// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class IMCPTool;

/**
 * Registry for MCP tools.
 * Manages tool registration, listing, and dispatch.
 * Does not hold any state — tools are stateless functions.
 */
class FMCPToolRegistry
{
public:
	/** Register a tool instance */
	void RegisterTool(TSharedPtr<IMCPTool> Tool);

	/** Get JSON array of all registered tools for tools/list response */
	TArray<TSharedPtr<FJsonValue>> GetToolsList() const;

	/** Call a tool by name. Returns the result content object, or nullptr if tool not found */
	TSharedPtr<FJsonObject> CallTool(const FString& ToolName, const TSharedPtr<FJsonObject>& Arguments) const;

	/** Check if a tool exists */
	bool HasTool(const FString& ToolName) const;

private:
	TMap<FString, TSharedPtr<IMCPTool>> Tools;
};
