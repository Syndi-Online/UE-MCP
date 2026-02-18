// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/MCPServerModule.h"
#include "Tools/MCPToolRegistry.h"
#include "Protocol/MCPSessionManager.h"
#include "Protocol/MCPJsonRpc.h"
#include "Protocol/MCPHttpServer.h"

// Tools
#include "Tools/Impl/HelloWorldImplTool.h"
#include "Tools/Impl/SpawnActorImplTool.h"
#include "Tools/Impl/DeleteActorImplTool.h"

// Modules
#include "Modules/Impl/ActorImplModule.h"

DEFINE_LOG_CATEGORY(LogMCPServer);

#define LOCTEXT_NAMESPACE "FMCPServerModule"

FMCPServerModule& FMCPServerModule::Get()
{
	return FModuleManager::GetModuleChecked<FMCPServerModule>("MCPServer");
}

void FMCPServerModule::StartupModule()
{
	// Create layers bottom-up: Modules -> Tools -> Protocol -> HTTP
	ActorModule = MakeUnique<FActorImplModule>();

	ToolRegistry = MakeUnique<FMCPToolRegistry>();
	RegisterBuiltinTools();

	SessionManager = MakeUnique<FMCPSessionManager>();
	JsonRpc = MakeUnique<FMCPJsonRpc>(*ToolRegistry, *SessionManager);
	HttpServer = MakeUnique<FMCPHttpServer>(*JsonRpc, *SessionManager);

	HttpServer->Start(ServerPort);

	UE_LOG(LogMCPServer, Log, TEXT("MCP Server plugin initialized on port %d"), ServerPort);
}

void FMCPServerModule::ShutdownModule()
{
	if (HttpServer.IsValid())
	{
		HttpServer->Stop();
	}

	HttpServer.Reset();
	JsonRpc.Reset();
	SessionManager.Reset();
	ToolRegistry.Reset();
	ActorModule.Reset();

	UE_LOG(LogMCPServer, Log, TEXT("MCP Server plugin shut down"));
}

FMCPToolRegistry& FMCPServerModule::GetToolRegistry()
{
	return *ToolRegistry;
}

void FMCPServerModule::RegisterBuiltinTools()
{
	ToolRegistry->RegisterTool(MakeShared<FHelloWorldImplTool>());
	ToolRegistry->RegisterTool(MakeShared<FSpawnActorImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FDeleteActorImplTool>(*ActorModule));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMCPServerModule, MCPServer)
