// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

class FMCPToolRegistry;
class FMCPSessionManager;

/**
 * Handles JSON-RPC 2.0 message parsing, serialization, and MCP method dispatch.
 * Delegates tool operations to FMCPToolRegistry and session operations to FMCPSessionManager.
 */
class FMCPJsonRpc
{
public:
	FMCPJsonRpc(FMCPToolRegistry& InToolRegistry, FMCPSessionManager& InSessionManager);

	/**
	 * Process an incoming JSON-RPC message body.
	 * @param RequestBody     Raw JSON string from HTTP body
	 * @param SessionId       Current session ID (empty if not yet established)
	 * @param OutResponse     The JSON response string to send back
	 * @param OutNewSessionId If non-empty, a new session was created
	 * @param OutHttpCode     HTTP status code to return
	 */
	void ProcessMessage(
		const FString& RequestBody,
		const FString& SessionId,
		FString& OutResponse,
		FString& OutNewSessionId,
		int32& OutHttpCode);

	/** JSON-RPC helpers */
	static TSharedPtr<FJsonObject> MakeResponse(const TSharedPtr<FJsonValue>& Id, const TSharedPtr<FJsonObject>& Result);
	static TSharedPtr<FJsonObject> MakeError(const TSharedPtr<FJsonValue>& Id, int32 Code, const FString& Message);
	static FString SerializeJson(const TSharedPtr<FJsonObject>& JsonObject);

private:
	FMCPToolRegistry& ToolRegistry;
	FMCPSessionManager& SessionManager;

	/** Handle a single JSON-RPC message (request or notification) */
	void HandleSingleMessage(
		const TSharedPtr<FJsonObject>& Message,
		const FString& SessionId,
		TSharedPtr<FJsonObject>& OutResponse,
		FString& OutNewSessionId,
		int32& OutHttpCode);

	/** MCP method handlers */
	TSharedPtr<FJsonObject> HandleInitialize(const TSharedPtr<FJsonValue>& Id, const TSharedPtr<FJsonObject>& Params, FString& OutNewSessionId);
	TSharedPtr<FJsonObject> HandleToolsList(const TSharedPtr<FJsonValue>& Id, const TSharedPtr<FJsonObject>& Params);
	TSharedPtr<FJsonObject> HandleToolsCall(const TSharedPtr<FJsonValue>& Id, const TSharedPtr<FJsonObject>& Params);
	TSharedPtr<FJsonObject> HandlePing(const TSharedPtr<FJsonValue>& Id);
};
