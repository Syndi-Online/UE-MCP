// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

/**
 * Base interface for all MCP tools.
 * Each tool represents a single callable function exposed to MCP clients.
 */
class IMCPTool
{
public:
	virtual ~IMCPTool() = default;

	/** Unique name of the tool (used in tools/call) */
	virtual FString GetName() const = 0;

	/** Human-readable description of what the tool does */
	virtual FString GetDescription() const = 0;

	/** JSON Schema describing the expected input arguments */
	virtual TSharedPtr<FJsonObject> GetInputSchema() const = 0;

	/**
	 * Execute the tool with the given arguments.
	 * @param Arguments  Parsed JSON arguments matching the input schema
	 * @return           Result object containing "content" array and "isError" flag
	 */
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) = 0;
};
