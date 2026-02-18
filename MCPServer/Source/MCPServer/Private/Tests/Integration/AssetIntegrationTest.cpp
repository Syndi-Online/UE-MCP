// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockAssetModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/CreateAssetImplTool.h"
#include "Tools/Impl/FindAssetsImplTool.h"
#include "Tools/Impl/SaveAssetImplTool.h"
#include "Tools/Impl/DuplicateAssetImplTool.h"
#include "Tools/Impl/RenameAssetImplTool.h"
#include "Tools/Impl/SetAssetMetadataImplTool.h"
#include "Tools/Impl/GetAssetMetadataImplTool.h"
#include "Tools/Impl/GetAssetDependenciesImplTool.h"
#include "Tools/Impl/DeleteAssetImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAssetIntegrationTest,
	"MCPServer.Integration.Asset.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAssetIntegrationTest::RunTest(const FString& Parameters)
{
	FMockAssetModule Mock;

	// Step 1: CreateAsset
	Mock.CreateAssetResult.bSuccess = true;
	Mock.CreateAssetResult.AssetName = TEXT("TestMaterial");
	Mock.CreateAssetResult.AssetPath = TEXT("/Game/Materials/TestMaterial");
	Mock.CreateAssetResult.AssetClass = TEXT("Material");

	FCreateAssetImplTool CreateTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_name"), TEXT("TestMaterial"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/Materials"));
	Args->SetStringField(TEXT("asset_class"), TEXT("Material"));
	auto Result = CreateTool.Execute(Args);
	TestTrue(TEXT("CreateAsset success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("CreateAsset result contains name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestMaterial")));

	// Step 2: FindAssets (verify)
	FAssetInfo FoundAsset;
	FoundAsset.AssetName = TEXT("TestMaterial");
	FoundAsset.AssetPath = TEXT("/Game/Materials/TestMaterial");
	FoundAsset.AssetClass = TEXT("Material");
	FoundAsset.PackagePath = TEXT("/Game/Materials");
	Mock.FindAssetsResult.bSuccess = true;
	Mock.FindAssetsResult.Assets.Add(FoundAsset);

	FFindAssetsImplTool FindTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/Materials"));
	Args->SetStringField(TEXT("class_name"), TEXT("Material"));
	Args->SetBoolField(TEXT("recursive"), false);
	Result = FindTool.Execute(Args);
	TestTrue(TEXT("FindAssets success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("FindAssets found our asset"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestMaterial")));

	// Step 3: SaveAsset
	Mock.SaveAssetResult.bSuccess = true;
	Mock.SaveAssetResult.AssetPath = TEXT("/Game/Materials/TestMaterial");

	FSaveAssetImplTool SaveTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Materials/TestMaterial"));
	TestTrue(TEXT("SaveAsset success"), MCPTestUtils::IsSuccess(SaveTool.Execute(Args)));

	// Step 4: DuplicateAsset
	Mock.DuplicateAssetResult.bSuccess = true;
	Mock.DuplicateAssetResult.AssetName = TEXT("TestMaterial_Copy");
	Mock.DuplicateAssetResult.AssetPath = TEXT("/Game/Materials/TestMaterial_Copy");

	FDuplicateAssetImplTool DupTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("source_path"), TEXT("/Game/Materials/TestMaterial"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/Materials/TestMaterial_Copy"));
	TestTrue(TEXT("DuplicateAsset success"), MCPTestUtils::IsSuccess(DupTool.Execute(Args)));

	// Step 5: RenameAsset
	Mock.RenameAssetResult.bSuccess = true;
	Mock.RenameAssetResult.NewPath = TEXT("/Game/Materials/TestMaterial_Renamed");

	FRenameAssetImplTool RenameTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("source_path"), TEXT("/Game/Materials/TestMaterial_Copy"));
	Args->SetStringField(TEXT("destination_path"), TEXT("/Game/Materials/TestMaterial_Renamed"));
	TestTrue(TEXT("RenameAsset success"), MCPTestUtils::IsSuccess(RenameTool.Execute(Args)));

	// Step 6: SetAssetMetadata
	Mock.SetAssetMetadataResult.bSuccess = true;

	FSetAssetMetadataImplTool SetMetaTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Materials/TestMaterial"));
	Args->SetStringField(TEXT("tag_name"), TEXT("Author"));
	Args->SetStringField(TEXT("tag_value"), TEXT("IntegrationTest"));
	TestTrue(TEXT("SetAssetMetadata success"), MCPTestUtils::IsSuccess(SetMetaTool.Execute(Args)));

	// Step 7: GetAssetMetadata
	Mock.GetAssetMetadataResult.bSuccess = true;
	Mock.GetAssetMetadataResult.Tags.Add(TEXT("Author"), TEXT("IntegrationTest"));

	FGetAssetMetadataImplTool GetMetaTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Materials/TestMaterial"));
	Result = GetMetaTool.Execute(Args);
	TestTrue(TEXT("GetAssetMetadata success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("GetAssetMetadata has tag"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Author")));

	// Step 8: GetAssetDependencies
	Mock.GetAssetDependenciesResult.bSuccess = true;
	Mock.GetAssetDependenciesResult.Dependencies.Add(TEXT("/Engine/EngineResources/DefaultTexture"));

	FGetAssetDependenciesImplTool DepsTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Materials/TestMaterial"));
	TestTrue(TEXT("GetAssetDependencies success"), MCPTestUtils::IsSuccess(DepsTool.Execute(Args)));

	// Step 9: DeleteAsset
	Mock.DeleteAssetResult.bSuccess = true;

	FDeleteAssetImplTool DeleteTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("asset_path"), TEXT("/Game/Materials/TestMaterial"));
	TestTrue(TEXT("DeleteAsset success"), MCPTestUtils::IsSuccess(DeleteTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("CreateAsset called"), Mock.Recorder.GetCallCount(TEXT("CreateAsset")), 1);
	TestEqual(TEXT("FindAssets called"), Mock.Recorder.GetCallCount(TEXT("FindAssets")), 1);
	TestEqual(TEXT("SaveAsset called"), Mock.Recorder.GetCallCount(TEXT("SaveAsset")), 1);
	TestEqual(TEXT("DuplicateAsset called"), Mock.Recorder.GetCallCount(TEXT("DuplicateAsset")), 1);
	TestEqual(TEXT("RenameAsset called"), Mock.Recorder.GetCallCount(TEXT("RenameAsset")), 1);
	TestEqual(TEXT("SetAssetMetadata called"), Mock.Recorder.GetCallCount(TEXT("SetAssetMetadata")), 1);
	TestEqual(TEXT("GetAssetMetadata called"), Mock.Recorder.GetCallCount(TEXT("GetAssetMetadata")), 1);
	TestEqual(TEXT("GetAssetDependencies called"), Mock.Recorder.GetCallCount(TEXT("GetAssetDependencies")), 1);
	TestEqual(TEXT("DeleteAsset called"), Mock.Recorder.GetCallCount(TEXT("DeleteAsset")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 9);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
