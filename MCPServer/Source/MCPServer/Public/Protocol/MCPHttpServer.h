// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HttpResultCallback.h"
#include "HttpRouteHandle.h"

class IHttpRouter;
class FMCPJsonRpc;
class FMCPSessionManager;

/**
 * HTTP server wrapper for MCP protocol.
 * Binds POST /mcp and DELETE /mcp routes, delegates message processing to FMCPJsonRpc.
 */
class FMCPHttpServer
{
public:
	FMCPHttpServer(FMCPJsonRpc& InJsonRpc, FMCPSessionManager& InSessionManager);

	/** Start listening on the given port */
	void Start(uint32 Port);

	/** Stop listening and unbind routes */
	void Stop();

private:
	/** Handle POST /mcp — incoming JSON-RPC messages */
	bool HandlePostRequest(const struct FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	/** Handle DELETE /mcp — session termination */
	bool HandleDeleteRequest(const struct FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);

	FMCPJsonRpc& JsonRpc;
	FMCPSessionManager& SessionManager;

	TSharedPtr<IHttpRouter> HttpRouter;
	FHttpRouteHandle PostRouteHandle;
	FHttpRouteHandle DeleteRouteHandle;
};
