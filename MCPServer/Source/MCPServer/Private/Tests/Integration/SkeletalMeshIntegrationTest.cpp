// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockSkeletalMeshModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/GetSkeletonInfoImplTool.h"
#include "Tools/Impl/GetPhysicsAssetImplTool.h"
#include "Tools/Impl/CreateAnimAssetImplTool.h"
#include "Tools/Impl/CreateAnimBlueprintImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSkeletalMeshIntegrationTest,
	"MCPServer.Integration.SkeletalMesh.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSkeletalMeshIntegrationTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;

	const FString MeshPath = TEXT("/Game/Characters/SK_Mannequin");
	const FString SkeletonPath = TEXT("/Game/Characters/SK_Mannequin_Skeleton");

	// Step 1: GetSkeletonInfo
	FBoneInfo RootBone;
	RootBone.BoneIndex = 0;
	RootBone.BoneName = TEXT("Root");
	RootBone.ParentIndex = -1;
	FBoneInfo PelvisBone;
	PelvisBone.BoneIndex = 1;
	PelvisBone.BoneName = TEXT("Pelvis");
	PelvisBone.ParentIndex = 0;

	Mock.GetSkeletonInfoResult.bSuccess = true;
	Mock.GetSkeletonInfoResult.NumBones = 67;
	Mock.GetSkeletonInfoResult.NumRawBones = 67;
	Mock.GetSkeletonInfoResult.SkeletonName = TEXT("SK_Mannequin_Skeleton");
	Mock.GetSkeletonInfoResult.Bones.Add(RootBone);
	Mock.GetSkeletonInfoResult.Bones.Add(PelvisBone);
	Mock.GetSkeletonInfoResult.LodCount = 2;
	Mock.GetSkeletonInfoResult.VertexCountPerLod.Add(10000);
	Mock.GetSkeletonInfoResult.VertexCountPerLod.Add(5000);

	FGetSkeletonInfoImplTool SkelInfoTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), MeshPath);
	auto Result = SkelInfoTool.Execute(Args);
	TestTrue(TEXT("GetSkeletonInfo success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("SkeletonInfo contains bone info"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Root")));

	// Step 2: GetPhysicsAsset
	Mock.GetPhysicsAssetResult.bSuccess = true;
	Mock.GetPhysicsAssetResult.PhysicsAssetPath = TEXT("/Game/Characters/SK_Mannequin_PhysicsAsset");
	Mock.GetPhysicsAssetResult.PhysicsAssetName = TEXT("SK_Mannequin_PhysicsAsset");
	Mock.GetPhysicsAssetResult.NumBodies = 15;
	Mock.GetPhysicsAssetResult.NumConstraints = 14;

	FGetPhysicsAssetImplTool PhysTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), MeshPath);
	Result = PhysTool.Execute(Args);
	TestTrue(TEXT("GetPhysicsAsset success"), MCPTestUtils::IsSuccess(Result));

	// Step 3: CreateAnimAsset
	Mock.CreateAnimAssetResult.bSuccess = true;
	Mock.CreateAnimAssetResult.AssetPath = TEXT("/Game/Animations/TestAnim");
	Mock.CreateAnimAssetResult.AssetName = TEXT("TestAnim");

	FCreateAnimAssetImplTool AnimTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("skeleton_path"), SkeletonPath);
	Args->SetStringField(TEXT("asset_name"), TEXT("TestAnim"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/Animations"));
	Args->SetStringField(TEXT("asset_type"), TEXT("AnimSequence"));
	Result = AnimTool.Execute(Args);
	TestTrue(TEXT("CreateAnimAsset success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("CreateAnimAsset result contains name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestAnim")));

	// Step 4: CreateAnimBlueprint
	Mock.CreateAnimBlueprintResult.bSuccess = true;
	Mock.CreateAnimBlueprintResult.AssetPath = TEXT("/Game/Animations/ABP_Mannequin");
	Mock.CreateAnimBlueprintResult.AssetName = TEXT("ABP_Mannequin");

	FCreateAnimBlueprintImplTool AnimBPTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("skeleton_path"), SkeletonPath);
	Args->SetStringField(TEXT("asset_name"), TEXT("ABP_Mannequin"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/Animations"));
	Result = AnimBPTool.Execute(Args);
	TestTrue(TEXT("CreateAnimBlueprint success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("CreateAnimBlueprint result contains name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("ABP_Mannequin")));

	// Verify all calls were made
	TestEqual(TEXT("GetSkeletonInfo called"), Mock.Recorder.GetCallCount(TEXT("GetSkeletonInfo")), 1);
	TestEqual(TEXT("GetPhysicsAsset called"), Mock.Recorder.GetCallCount(TEXT("GetPhysicsAsset")), 1);
	TestEqual(TEXT("CreateAnimAsset called"), Mock.Recorder.GetCallCount(TEXT("CreateAnimAsset")), 1);
	TestEqual(TEXT("CreateAnimBlueprint called"), Mock.Recorder.GetCallCount(TEXT("CreateAnimBlueprint")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 4);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
