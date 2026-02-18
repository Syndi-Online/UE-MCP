// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMCPToolRegistry;
class FMCPSessionManager;
class FMCPJsonRpc;
class FMCPHttpServer;
class IActorModule;

DECLARE_LOG_CATEGORY_EXTERN(LogMCPServer, Log, All);

/**
 * MCP Server plugin module.
 * Entry point that wires together Protocol, Tools, and Modules layers.
 */
class FMCPServerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static FMCPServerModule& Get();

	/** Access the tool registry for external tool registration */
	FMCPToolRegistry& GetToolRegistry();

private:
	/** Register all built-in tools */
	void RegisterBuiltinTools();

	uint32 ServerPort = 8080;

	// Modules layer
	TUniquePtr<IActorModule> ActorModule;

	// Tools layer
	TUniquePtr<FMCPToolRegistry> ToolRegistry;

	// Protocol layer
	TUniquePtr<FMCPSessionManager> SessionManager;
	TUniquePtr<FMCPJsonRpc> JsonRpc;
	TUniquePtr<FMCPHttpServer> HttpServer;
};
