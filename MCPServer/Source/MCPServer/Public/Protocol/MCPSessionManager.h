// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

/**
 * Manages MCP session lifecycle.
 * Sessions are identified by unique string IDs passed via Mcp-Session-Id header.
 */
class FMCPSessionManager
{
public:
	/** Create a new session and return its ID */
	FString CreateSession();

	/** Check if a session ID is valid and active */
	bool IsValidSession(const FString& SessionId) const;

	/** Remove a session by ID */
	void RemoveSession(const FString& SessionId);

private:
	TSet<FString> ActiveSessions;
};
