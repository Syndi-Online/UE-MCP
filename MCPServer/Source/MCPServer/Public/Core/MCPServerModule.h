// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMCPToolRegistry;
class FMCPSessionManager;
class FMCPJsonRpc;
class FMCPHttpServer;
class IActorModule;
class ILevelModule;
class IAssetModule;
class IViewportModule;
class IBlueprintModule;
class IMaterialModule;
class IStaticMeshModule;
class ISkeletalMeshModule;
class ISequencerToolModule;
class ILandscapeToolModule;
class IFoliageModule;
class INavigationModule;
class IPhysicsModule;
class IBuildModule;
class IConsoleModule;

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
	TUniquePtr<ILevelModule> LevelModule;
	TUniquePtr<IAssetModule> AssetModule;
	TUniquePtr<IViewportModule> ViewportModule;
	TUniquePtr<IBlueprintModule> BlueprintModule;
	TUniquePtr<IMaterialModule> MaterialModule;
	TUniquePtr<IStaticMeshModule> StaticMeshModule;
	TUniquePtr<ISkeletalMeshModule> SkeletalMeshModule;
	TUniquePtr<ISequencerToolModule> SequencerModule;
	TUniquePtr<ILandscapeToolModule> LandscapeModule;
	TUniquePtr<IFoliageModule> FoliageModule;
	TUniquePtr<INavigationModule> NavigationModule;
	TUniquePtr<IPhysicsModule> PhysicsModule;
	TUniquePtr<IBuildModule> BuildModule;
	TUniquePtr<IConsoleModule> ConsoleModule;

	// Tools layer
	TUniquePtr<FMCPToolRegistry> ToolRegistry;

	// Protocol layer
	TUniquePtr<FMCPSessionManager> SessionManager;
	TUniquePtr<FMCPJsonRpc> JsonRpc;
	TUniquePtr<FMCPHttpServer> HttpServer;
};
