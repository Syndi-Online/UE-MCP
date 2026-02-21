// Copyright Epic Games, Inc. All Rights Reserved.

#include "Core/MCPServerModule.h"
#include "Tools/MCPToolRegistry.h"
#include "Protocol/MCPSessionManager.h"
#include "Protocol/MCPJsonRpc.h"
#include "Protocol/MCPHttpServer.h"

// Tools
#include "Tools/Impl/BatchSetActorTransformsImplTool.h"
#include "Tools/Impl/BatchSetActorFoldersImplTool.h"
#include "Tools/Impl/BatchSetActorPropertiesImplTool.h"
#include "Tools/Impl/BatchSetMaterialExpressionPropertiesImplTool.h"
#include "Tools/Impl/BatchConnectMaterialExpressionsImplTool.h"
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
#include "Tools/Impl/NewLevelImplTool.h"
#include "Tools/Impl/NewLevelFromTemplateImplTool.h"
#include "Tools/Impl/LoadLevelImplTool.h"
#include "Tools/Impl/SaveLevelImplTool.h"
#include "Tools/Impl/SaveAllDirtyLevelsImplTool.h"
#include "Tools/Impl/AddSublevelImplTool.h"
#include "Tools/Impl/RemoveSublevelImplTool.h"
#include "Tools/Impl/SetCurrentLevelImplTool.h"
#include "Tools/Impl/SetLevelVisibilityImplTool.h"
#include "Tools/Impl/LoadAssetImplTool.h"
#include "Tools/Impl/CreateAssetImplTool.h"
#include "Tools/Impl/DuplicateAssetImplTool.h"
#include "Tools/Impl/RenameAssetImplTool.h"
#include "Tools/Impl/DeleteAssetImplTool.h"
#include "Tools/Impl/SaveAssetImplTool.h"
#include "Tools/Impl/FindAssetsImplTool.h"
#include "Tools/Impl/ListAssetsImplTool.h"
#include "Tools/Impl/ImportAssetImplTool.h"
#include "Tools/Impl/ExportAssetImplTool.h"
#include "Tools/Impl/GetAssetDependenciesImplTool.h"
#include "Tools/Impl/GetAssetReferencersImplTool.h"
#include "Tools/Impl/GetAssetMetadataImplTool.h"
#include "Tools/Impl/SetAssetMetadataImplTool.h"
#include "Tools/Impl/SetAssetPropertyImplTool.h"
#include "Tools/Impl/GetAssetPropertyImplTool.h"
#include "Tools/Impl/FindReferencersOfClassImplTool.h"
#include "Tools/Impl/GetViewportCameraImplTool.h"
#include "Tools/Impl/SetViewportCameraImplTool.h"
#include "Tools/Impl/SetGameViewImplTool.h"
#include "Tools/Impl/SetViewportRealtimeImplTool.h"
#include "Tools/Impl/SetViewModeImplTool.h"
#include "Tools/Impl/SetCameraFovImplTool.h"
#include "Tools/Impl/FocusOnActorsImplTool.h"
#include "Tools/Impl/TakeScreenshotImplTool.h"
#include "Tools/Impl/PilotActorImplTool.h"
#include "Tools/Impl/EjectPilotImplTool.h"
#include "Tools/Impl/SetBookmarkImplTool.h"
#include "Tools/Impl/JumpToBookmarkImplTool.h"
#include "Tools/Impl/CreateBlueprintImplTool.h"
#include "Tools/Impl/CreateBlueprintFromActorImplTool.h"
#include "Tools/Impl/CompileBlueprintImplTool.h"
#include "Tools/Impl/AddBlueprintVariableImplTool.h"
#include "Tools/Impl/RemoveBlueprintVariableImplTool.h"
#include "Tools/Impl/AddBlueprintFunctionImplTool.h"
#include "Tools/Impl/AddBlueprintInterfaceImplTool.h"
#include "Tools/Impl/GetBlueprintGraphsImplTool.h"
#include "Tools/Impl/ReparentBlueprintImplTool.h"
#include "Tools/Impl/OpenBlueprintEditorImplTool.h"
#include "Tools/Impl/GetGraphNodesImplTool.h"
#include "Tools/Impl/GetGraphNodesSummaryImplTool.h"
#include "Tools/Impl/SetNodePositionImplTool.h"
#include "Tools/Impl/BatchSetNodePositionsImplTool.h"
#include "Tools/Impl/AddCommentBoxImplTool.h"
#include "Tools/Impl/DeleteCommentBoxImplTool.h"
#include "Tools/Impl/SetCommentBoxPropertiesImplTool.h"
#include "Tools/Impl/AddBlueprintComponentImplTool.h"
#include "Tools/Impl/RemoveBlueprintComponentImplTool.h"
#include "Tools/Impl/GetBlueprintComponentsImplTool.h"
#include "Tools/Impl/SetBlueprintComponentPropertyImplTool.h"
#include "Tools/Impl/GetBlueprintComponentPropertyImplTool.h"
#include "Tools/Impl/AddGraphNodeImplTool.h"
#include "Tools/Impl/ConnectGraphPinsImplTool.h"
#include "Tools/Impl/SetPinDefaultValueImplTool.h"
#include "Tools/Impl/DeleteGraphNodeImplTool.h"
#include "Tools/Impl/AddEventDispatcherImplTool.h"
#include "Tools/Impl/GetBlueprintParentClassImplTool.h"
#include "Tools/Impl/AddGraphNodesBatchImplTool.h"
#include "Tools/Impl/GetGraphNodesInAreaImplTool.h"
#include "Tools/Impl/DisconnectGraphPinsImplTool.h"
#include "Tools/Impl/FindFunctionImplTool.h"
#include "Tools/Impl/BatchConnectGraphPinsImplTool.h"
#include "Tools/Impl/BatchSetPinDefaultsImplTool.h"
#include "Tools/Impl/BatchDeleteGraphNodesImplTool.h"
#include "Tools/Impl/BatchDisconnectGraphPinsImplTool.h"
#include "Tools/Impl/CreateMaterialExpressionImplTool.h"
#include "Tools/Impl/DeleteMaterialExpressionImplTool.h"
#include "Tools/Impl/ConnectMaterialExpressionsImplTool.h"
#include "Tools/Impl/ConnectMaterialPropertyImplTool.h"
#include "Tools/Impl/RecompileMaterialImplTool.h"
#include "Tools/Impl/GetMaterialParameterImplTool.h"
#include "Tools/Impl/SetMaterialInstanceParameterImplTool.h"
#include "Tools/Impl/SetMaterialInstanceParentImplTool.h"
#include "Tools/Impl/GetMaterialStatisticsImplTool.h"
#include "Tools/Impl/GetMaterialExpressionsImplTool.h"
#include "Tools/Impl/GetMaterialExpressionPropertyImplTool.h"
#include "Tools/Impl/SetMaterialExpressionPropertyImplTool.h"
#include "Tools/Impl/SetStaticMeshLodImplTool.h"
#include "Tools/Impl/ImportLodImplTool.h"
#include "Tools/Impl/GetLodSettingsImplTool.h"
#include "Tools/Impl/SetCollisionImplTool.h"
#include "Tools/Impl/AddUVChannelImplTool.h"
#include "Tools/Impl/RemoveUVChannelImplTool.h"
#include "Tools/Impl/GenerateUVChannelImplTool.h"
#include "Tools/Impl/SetMeshMaterialImplTool.h"
#include "Tools/Impl/GetMeshBoundsImplTool.h"
#include "Tools/Impl/RegenerateSkeletalLodImplTool.h"
#include "Tools/Impl/GetSkeletonInfoImplTool.h"
#include "Tools/Impl/GetPhysicsAssetImplTool.h"
#include "Tools/Impl/SetPhysicsAssetImplTool.h"
#include "Tools/Impl/ReimportSkeletalMeshImplTool.h"
#include "Tools/Impl/CreateAnimAssetImplTool.h"
#include "Tools/Impl/CreateAnimBlueprintImplTool.h"
#include "Tools/Impl/ApplyAnimCompressionImplTool.h"
#include "Tools/Impl/SequencerPlayImplTool.h"
#include "Tools/Impl/SequencerPauseImplTool.h"
#include "Tools/Impl/SequencerStopImplTool.h"
#include "Tools/Impl/SequencerSetTimeImplTool.h"
#include "Tools/Impl/SequencerGetTimeImplTool.h"
#include "Tools/Impl/SequencerSetPlaybackRangeImplTool.h"
#include "Tools/Impl/SequencerAddActorImplTool.h"
#include "Tools/Impl/SequencerAddTrackImplTool.h"
#include "Tools/Impl/SequencerAddMarkerImplTool.h"
#include "Tools/Impl/SequencerForceEvaluateImplTool.h"
#include "Tools/Impl/GetLandscapeHeightmapImplTool.h"
#include "Tools/Impl/SetLandscapeHeightmapImplTool.h"
#include "Tools/Impl/GetLandscapeWeightmapImplTool.h"
#include "Tools/Impl/SetLandscapeWeightmapImplTool.h"
#include "Tools/Impl/ImportLandscapeImplTool.h"
#include "Tools/Impl/ExportLandscapeImplTool.h"
#include "Tools/Impl/RebuildLandscapeImplTool.h"
#include "Tools/Impl/AddFoliageInstancesImplTool.h"
#include "Tools/Impl/RemoveFoliageInstancesImplTool.h"
#include "Tools/Impl/RegisterFoliageMeshImplTool.h"
#include "Tools/Impl/ResimulateProceduralFoliageImplTool.h"
#include "Tools/Impl/BuildNavigationImplTool.h"
#include "Tools/Impl/RebuildNavigationImplTool.h"
#include "Tools/Impl/FindPathImplTool.h"
#include "Tools/Impl/ProjectPointToNavImplTool.h"
#include "Tools/Impl/GetRandomReachablePointImplTool.h"
#include "Tools/Impl/CreatePhysicsCollisionImplTool.h"
#include "Tools/Impl/SetPhysicalMaterialImplTool.h"
#include "Tools/Impl/SetPhysicsVelocityImplTool.h"
#include "Tools/Impl/ManageConstraintsImplTool.h"
#include "Tools/Impl/BuildGeometryImplTool.h"
#include "Tools/Impl/BuildLightingImplTool.h"
#include "Tools/Impl/BuildNavigationDataImplTool.h"
#include "Tools/Impl/BuildHlodImplTool.h"
#include "Tools/Impl/BuildTextureStreamingImplTool.h"
#include "Tools/Impl/BuildAllImplTool.h"
#include "Tools/Impl/GetBuildStatusImplTool.h"
#include "Tools/Impl/ExecuteConsoleCommandImplTool.h"
#include "Tools/Impl/GetCvarImplTool.h"
#include "Tools/Impl/SetCvarImplTool.h"
#include "Tools/Impl/GetProjectInfoImplTool.h"
#include "Tools/Impl/GetConfigValueImplTool.h"
#include "Tools/Impl/SetConfigValueImplTool.h"
#include "Tools/Impl/GetProjectPathsImplTool.h"
#include "Tools/Impl/SccCheckoutImplTool.h"
#include "Tools/Impl/SccAddImplTool.h"
#include "Tools/Impl/SccDeleteImplTool.h"
#include "Tools/Impl/SccRevertImplTool.h"
#include "Tools/Impl/SccStatusImplTool.h"
#include "Tools/Impl/SccSubmitImplTool.h"
#include "Tools/Impl/SccSyncImplTool.h"
#include "Tools/Impl/ShowNotificationImplTool.h"
#include "Tools/Impl/ShowDialogImplTool.h"
#include "Tools/Impl/OpenAssetEditorImplTool.h"
#include "Tools/Impl/CloseAssetEditorImplTool.h"
#include "Tools/Impl/SyncContentBrowserImplTool.h"
#include "Tools/Impl/GetSelectedContentBrowserImplTool.h"
#include "Tools/Impl/BuildHlodWpImplTool.h"
#include "Tools/Impl/DeleteHlodWpImplTool.h"
#include "Tools/Impl/PlayInEditorImplTool.h"
#include "Tools/Impl/StopPieImplTool.h"
#include "Tools/Impl/SimulateInEditorImplTool.h"
#include "Tools/Impl/IsPlayingImplTool.h"
#include "Tools/Impl/CreateWidgetBlueprintImplTool.h"
#include "Tools/Impl/GetWidgetTreeImplTool.h"
#include "Tools/Impl/AddWidgetImplTool.h"
#include "Tools/Impl/RemoveWidgetImplTool.h"
#include "Tools/Impl/MoveWidgetImplTool.h"
#include "Tools/Impl/RenameWidgetImplTool.h"
#include "Tools/Impl/ReplaceWidgetImplTool.h"
#include "Tools/Impl/SetWidgetPropertyImplTool.h"
#include "Tools/Impl/GetWidgetPropertyImplTool.h"
#include "Tools/Impl/SetWidgetSlotImplTool.h"
#include "Tools/Impl/GetWidgetAnimationsImplTool.h"
#include "Tools/Impl/ExportWidgetsImplTool.h"
#include "Tools/Impl/ImportWidgetsImplTool.h"

// Modules
#include "Modules/Impl/ActorImplModule.h"
#include "Modules/Impl/LevelImplModule.h"
#include "Modules/Impl/AssetImplModule.h"
#include "Modules/Impl/ViewportImplModule.h"
#include "Modules/Impl/BlueprintImplModule.h"
#include "Modules/Impl/MaterialImplModule.h"
#include "Modules/Impl/StaticMeshImplModule.h"
#include "Modules/Impl/SkeletalMeshImplModule.h"
#include "Modules/Impl/SequencerToolImplModule.h"
#include "Modules/Impl/LandscapeToolImplModule.h"
#include "Modules/Impl/FoliageImplModule.h"
#include "Modules/Impl/NavigationImplModule.h"
#include "Modules/Impl/PhysicsImplModule.h"
#include "Modules/Impl/BuildImplModule.h"
#include "Modules/Impl/ConsoleImplModule.h"
#include "Modules/Impl/ProjectSettingsImplModule.h"
#include "Modules/Impl/SourceControlImplModule.h"
#include "Modules/Impl/UIImplModule.h"
#include "Modules/Impl/WorldPartitionImplModule.h"
#include "Modules/Impl/PIEImplModule.h"
#include "Modules/Impl/UMGImplModule.h"

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
	LevelModule = MakeUnique<FLevelImplModule>();
	AssetModule = MakeUnique<FAssetImplModule>();
	ViewportModule = MakeUnique<FViewportImplModule>(*ActorModule);
	BlueprintModule = MakeUnique<FBlueprintImplModule>(*ActorModule);
	MaterialModule = MakeUnique<FMaterialImplModule>();
	StaticMeshModule = MakeUnique<FStaticMeshImplModule>();
	SkeletalMeshModule = MakeUnique<FSkeletalMeshImplModule>();
	SequencerModule = MakeUnique<FSequencerToolImplModule>();
	LandscapeModule = MakeUnique<FLandscapeToolImplModule>();
	FoliageModule = MakeUnique<FFoliageImplModule>();
	NavigationModule = MakeUnique<FNavigationImplModule>();
	PhysicsModule = MakeUnique<FPhysicsImplModule>(*ActorModule);
	BuildModule = MakeUnique<FBuildImplModule>();
	ConsoleModule = MakeUnique<FConsoleImplModule>();
	ProjectSettingsModule = MakeUnique<FProjectSettingsImplModule>();
	SCCModule = MakeUnique<FSourceControlImplModule>();
	UIModule = MakeUnique<FUIImplModule>();
	WorldPartitionModule = MakeUnique<FWorldPartitionImplModule>();
	PIEModule = MakeUnique<FPIEImplModule>();
	UMGModule = MakeUnique<FUMGImplModule>();

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
	UMGModule.Reset();
	PIEModule.Reset();
	WorldPartitionModule.Reset();
	UIModule.Reset();
	SCCModule.Reset();
	ProjectSettingsModule.Reset();
	ConsoleModule.Reset();
	BuildModule.Reset();
	PhysicsModule.Reset();
	NavigationModule.Reset();
	FoliageModule.Reset();
	LandscapeModule.Reset();
	SequencerModule.Reset();
	SkeletalMeshModule.Reset();
	StaticMeshModule.Reset();
	MaterialModule.Reset();
	BlueprintModule.Reset();
	ViewportModule.Reset();
	AssetModule.Reset();
	LevelModule.Reset();
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

	// Level management tools
	ToolRegistry->RegisterTool(MakeShared<FNewLevelImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FNewLevelFromTemplateImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FLoadLevelImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FSaveLevelImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FSaveAllDirtyLevelsImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FAddSublevelImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FRemoveSublevelImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FSetCurrentLevelImplTool>(*LevelModule));
	ToolRegistry->RegisterTool(MakeShared<FSetLevelVisibilityImplTool>(*LevelModule));

	// Asset management tools
	ToolRegistry->RegisterTool(MakeShared<FLoadAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FCreateAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FDuplicateAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FRenameAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FDeleteAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FSaveAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FFindAssetsImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FListAssetsImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FImportAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FExportAssetImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FGetAssetDependenciesImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FGetAssetReferencersImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FGetAssetMetadataImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FSetAssetMetadataImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FSetAssetPropertyImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FGetAssetPropertyImplTool>(*AssetModule));
	ToolRegistry->RegisterTool(MakeShared<FFindReferencersOfClassImplTool>(*AssetModule));

	// Viewport and camera tools
	ToolRegistry->RegisterTool(MakeShared<FGetViewportCameraImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FSetViewportCameraImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FSetGameViewImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FSetViewportRealtimeImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FSetViewModeImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FSetCameraFovImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FFocusOnActorsImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FTakeScreenshotImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FPilotActorImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FEjectPilotImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FSetBookmarkImplTool>(*ViewportModule));
	ToolRegistry->RegisterTool(MakeShared<FJumpToBookmarkImplTool>(*ViewportModule));

	// Blueprint tools
	ToolRegistry->RegisterTool(MakeShared<FCreateBlueprintImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FCreateBlueprintFromActorImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FCompileBlueprintImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FAddBlueprintVariableImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FRemoveBlueprintVariableImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FAddBlueprintFunctionImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FAddBlueprintInterfaceImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FGetBlueprintGraphsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FReparentBlueprintImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FOpenBlueprintEditorImplTool>(*BlueprintModule));

	// Blueprint graph node tools
	ToolRegistry->RegisterTool(MakeShared<FGetGraphNodesImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FGetGraphNodesSummaryImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FSetNodePositionImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchSetNodePositionsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FAddCommentBoxImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FDeleteCommentBoxImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FSetCommentBoxPropertiesImplTool>(*BlueprintModule));

	// Blueprint component tools
	ToolRegistry->RegisterTool(MakeShared<FAddBlueprintComponentImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FRemoveBlueprintComponentImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FGetBlueprintComponentsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FSetBlueprintComponentPropertyImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FGetBlueprintComponentPropertyImplTool>(*BlueprintModule));

	// Blueprint graph editing tools
	ToolRegistry->RegisterTool(MakeShared<FAddGraphNodeImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FConnectGraphPinsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FSetPinDefaultValueImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FDeleteGraphNodeImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FAddEventDispatcherImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FGetBlueprintParentClassImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FAddGraphNodesBatchImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FGetGraphNodesInAreaImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FDisconnectGraphPinsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FFindFunctionImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchConnectGraphPinsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchSetPinDefaultsImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchDeleteGraphNodesImplTool>(*BlueprintModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchDisconnectGraphPinsImplTool>(*BlueprintModule));

	// Material tools
	ToolRegistry->RegisterTool(MakeShared<FCreateMaterialExpressionImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FDeleteMaterialExpressionImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FConnectMaterialExpressionsImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FConnectMaterialPropertyImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FRecompileMaterialImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FGetMaterialParameterImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FSetMaterialInstanceParameterImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FSetMaterialInstanceParentImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FGetMaterialStatisticsImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FGetMaterialExpressionsImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FGetMaterialExpressionPropertyImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FSetMaterialExpressionPropertyImplTool>(*MaterialModule));

	// Static mesh tools
	ToolRegistry->RegisterTool(MakeShared<FSetStaticMeshLodImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FImportLodImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FGetLodSettingsImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FSetCollisionImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FAddUVChannelImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FRemoveUVChannelImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FGenerateUVChannelImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FSetMeshMaterialImplTool>(*StaticMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FGetMeshBoundsImplTool>(*StaticMeshModule));

	// Skeletal mesh and animation tools
	ToolRegistry->RegisterTool(MakeShared<FRegenerateSkeletalLodImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FGetSkeletonInfoImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FGetPhysicsAssetImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FSetPhysicsAssetImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FReimportSkeletalMeshImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FCreateAnimAssetImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FCreateAnimBlueprintImplTool>(*SkeletalMeshModule));
	ToolRegistry->RegisterTool(MakeShared<FApplyAnimCompressionImplTool>(*SkeletalMeshModule));

	// Sequencer / Cinematics tools
	ToolRegistry->RegisterTool(MakeShared<FSequencerPlayImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerPauseImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerStopImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerSetTimeImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerGetTimeImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerSetPlaybackRangeImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerAddActorImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerAddTrackImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerAddMarkerImplTool>(*SequencerModule));
	ToolRegistry->RegisterTool(MakeShared<FSequencerForceEvaluateImplTool>(*SequencerModule));

	// Landscape tools
	ToolRegistry->RegisterTool(MakeShared<FGetLandscapeHeightmapImplTool>(*LandscapeModule));
	ToolRegistry->RegisterTool(MakeShared<FSetLandscapeHeightmapImplTool>(*LandscapeModule));
	ToolRegistry->RegisterTool(MakeShared<FGetLandscapeWeightmapImplTool>(*LandscapeModule));
	ToolRegistry->RegisterTool(MakeShared<FSetLandscapeWeightmapImplTool>(*LandscapeModule));
	ToolRegistry->RegisterTool(MakeShared<FImportLandscapeImplTool>(*LandscapeModule));
	ToolRegistry->RegisterTool(MakeShared<FExportLandscapeImplTool>(*LandscapeModule));
	ToolRegistry->RegisterTool(MakeShared<FRebuildLandscapeImplTool>(*LandscapeModule));

	// Foliage tools
	ToolRegistry->RegisterTool(MakeShared<FAddFoliageInstancesImplTool>(*FoliageModule));
	ToolRegistry->RegisterTool(MakeShared<FRemoveFoliageInstancesImplTool>(*FoliageModule));
	ToolRegistry->RegisterTool(MakeShared<FRegisterFoliageMeshImplTool>(*FoliageModule));
	ToolRegistry->RegisterTool(MakeShared<FResimulateProceduralFoliageImplTool>(*FoliageModule));

	// Navigation / AI tools
	ToolRegistry->RegisterTool(MakeShared<FBuildNavigationImplTool>(*NavigationModule));
	ToolRegistry->RegisterTool(MakeShared<FRebuildNavigationImplTool>(*NavigationModule));
	ToolRegistry->RegisterTool(MakeShared<FFindPathImplTool>(*NavigationModule));
	ToolRegistry->RegisterTool(MakeShared<FProjectPointToNavImplTool>(*NavigationModule));
	ToolRegistry->RegisterTool(MakeShared<FGetRandomReachablePointImplTool>(*NavigationModule));

	// Physics tools
	ToolRegistry->RegisterTool(MakeShared<FCreatePhysicsCollisionImplTool>(*PhysicsModule));
	ToolRegistry->RegisterTool(MakeShared<FSetPhysicalMaterialImplTool>(*PhysicsModule));
	ToolRegistry->RegisterTool(MakeShared<FSetPhysicsVelocityImplTool>(*PhysicsModule));
	ToolRegistry->RegisterTool(MakeShared<FManageConstraintsImplTool>(*PhysicsModule));

	// Build tools
	ToolRegistry->RegisterTool(MakeShared<FBuildGeometryImplTool>(*BuildModule));
	ToolRegistry->RegisterTool(MakeShared<FBuildLightingImplTool>(*BuildModule));
	ToolRegistry->RegisterTool(MakeShared<FBuildNavigationDataImplTool>(*BuildModule));
	ToolRegistry->RegisterTool(MakeShared<FBuildHlodImplTool>(*BuildModule));
	ToolRegistry->RegisterTool(MakeShared<FBuildTextureStreamingImplTool>(*BuildModule));
	ToolRegistry->RegisterTool(MakeShared<FBuildAllImplTool>(*BuildModule));
	ToolRegistry->RegisterTool(MakeShared<FGetBuildStatusImplTool>(*BuildModule));

	// Console command tools
	ToolRegistry->RegisterTool(MakeShared<FExecuteConsoleCommandImplTool>(*ConsoleModule));
	ToolRegistry->RegisterTool(MakeShared<FGetCvarImplTool>(*ConsoleModule));
	ToolRegistry->RegisterTool(MakeShared<FSetCvarImplTool>(*ConsoleModule));

	// Project settings tools
	ToolRegistry->RegisterTool(MakeShared<FGetProjectInfoImplTool>(*ProjectSettingsModule));
	ToolRegistry->RegisterTool(MakeShared<FGetConfigValueImplTool>(*ProjectSettingsModule));
	ToolRegistry->RegisterTool(MakeShared<FSetConfigValueImplTool>(*ProjectSettingsModule));
	ToolRegistry->RegisterTool(MakeShared<FGetProjectPathsImplTool>(*ProjectSettingsModule));

	// Source control tools
	ToolRegistry->RegisterTool(MakeShared<FSccCheckoutImplTool>(*SCCModule));
	ToolRegistry->RegisterTool(MakeShared<FSccAddImplTool>(*SCCModule));
	ToolRegistry->RegisterTool(MakeShared<FSccDeleteImplTool>(*SCCModule));
	ToolRegistry->RegisterTool(MakeShared<FSccRevertImplTool>(*SCCModule));
	ToolRegistry->RegisterTool(MakeShared<FSccStatusImplTool>(*SCCModule));
	ToolRegistry->RegisterTool(MakeShared<FSccSubmitImplTool>(*SCCModule));
	ToolRegistry->RegisterTool(MakeShared<FSccSyncImplTool>(*SCCModule));

	// UI and notification tools
	ToolRegistry->RegisterTool(MakeShared<FShowNotificationImplTool>(*UIModule));
	ToolRegistry->RegisterTool(MakeShared<FShowDialogImplTool>(*UIModule));
	ToolRegistry->RegisterTool(MakeShared<FOpenAssetEditorImplTool>(*UIModule));
	ToolRegistry->RegisterTool(MakeShared<FCloseAssetEditorImplTool>(*UIModule));
	ToolRegistry->RegisterTool(MakeShared<FSyncContentBrowserImplTool>(*UIModule));
	ToolRegistry->RegisterTool(MakeShared<FGetSelectedContentBrowserImplTool>(*UIModule));

	// World Partition tools
	ToolRegistry->RegisterTool(MakeShared<FBuildHlodWpImplTool>(*WorldPartitionModule));
	ToolRegistry->RegisterTool(MakeShared<FDeleteHlodWpImplTool>(*WorldPartitionModule));

	// PIE (Play In Editor) tools
	ToolRegistry->RegisterTool(MakeShared<FPlayInEditorImplTool>(*PIEModule));
	ToolRegistry->RegisterTool(MakeShared<FStopPieImplTool>(*PIEModule));
	ToolRegistry->RegisterTool(MakeShared<FSimulateInEditorImplTool>(*PIEModule));
	ToolRegistry->RegisterTool(MakeShared<FIsPlayingImplTool>(*PIEModule));

	// UMG Widget tools
	ToolRegistry->RegisterTool(MakeShared<FCreateWidgetBlueprintImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FGetWidgetTreeImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FAddWidgetImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FRemoveWidgetImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FMoveWidgetImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FRenameWidgetImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FReplaceWidgetImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FSetWidgetPropertyImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FGetWidgetPropertyImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FSetWidgetSlotImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FGetWidgetAnimationsImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FExportWidgetsImplTool>(*UMGModule));
	ToolRegistry->RegisterTool(MakeShared<FImportWidgetsImplTool>(*UMGModule));

	// Batch tools
	ToolRegistry->RegisterTool(MakeShared<FBatchSetActorTransformsImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchSetActorFoldersImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchSetActorPropertiesImplTool>(*ActorModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchSetMaterialExpressionPropertiesImplTool>(*MaterialModule));
	ToolRegistry->RegisterTool(MakeShared<FBatchConnectMaterialExpressionsImplTool>(*MaterialModule));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMCPServerModule, MCPServer)
