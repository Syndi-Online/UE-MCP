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
#include "Tools/Impl/DuplicateActorImplTool.h"
#include "Tools/Impl/GetActorsInLevelImplTool.h"
#include "Tools/Impl/SelectActorImplTool.h"
#include "Tools/Impl/DeselectAllImplTool.h"
#include "Tools/Impl/GetActorTransformImplTool.h"
#include "Tools/Impl/SetActorTransformImplTool.h"
#include "Tools/Impl/GetActorPropertyImplTool.h"
#include "Tools/Impl/SetActorPropertyImplTool.h"
#include "Tools/Impl/ConvertActorImplTool.h"
#include "Tools/Impl/MoveActorsToLevelImplTool.h"
#include "Tools/Impl/GroupActorsImplTool.h"
#include "Tools/Impl/SetActorFolderImplTool.h"

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
	ToolRegistry->RegisterTool(MakeShared<FDuplicateActorImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FGetActorsInLevelImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FSelectActorImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FDeselectAllImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FGetActorTransformImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FSetActorTransformImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FGetActorPropertyImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FSetActorPropertyImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FConvertActorImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FMoveActorsToLevelImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FGroupActorsImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FSetActorFolderImplTool>(*ActorModule));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMCPServerModule, MCPServer)
