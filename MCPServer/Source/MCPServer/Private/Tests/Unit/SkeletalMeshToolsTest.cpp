// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/RegenerateSkeletalLodImplTool.h"
#include "Tools/Impl/GetSkeletonInfoImplTool.h"
#include "Tools/Impl/GetPhysicsAssetImplTool.h"
#include "Tools/Impl/SetPhysicsAssetImplTool.h"
#include "Tools/Impl/ReimportSkeletalMeshImplTool.h"
#include "Tools/Impl/CreateAnimAssetImplTool.h"
#include "Tools/Impl/CreateAnimBlueprintImplTool.h"
#include "Tools/Impl/ApplyAnimCompressionImplTool.h"
#include "Tests/Mocks/MockSkeletalMeshModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ---------------------------------------------------------------------------
// RegenerateSkeletalLod
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegenerateSkeletalLodMetadataTest,
	"MCPServer.Unit.SkeletalMesh.RegenerateSkeletalLod.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegenerateSkeletalLodMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FRegenerateSkeletalLodImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("regenerate_skeletal_lod"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegenerateSkeletalLodSuccessTest,
	"MCPServer.Unit.SkeletalMesh.RegenerateSkeletalLod.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegenerateSkeletalLodSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.RegenerateSkeletalLodResult.bSuccess = true;
	Mock.RegenerateSkeletalLodResult.NewLodCount = 3;
	FRegenerateSkeletalLodImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	Args->SetNumberField(TEXT("lod_count"), 3);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains NewLodCount 3"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegenerateSkeletalLodMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.RegenerateSkeletalLod.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegenerateSkeletalLodMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FRegenerateSkeletalLodImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegenerateSkeletalLodModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.RegenerateSkeletalLod.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegenerateSkeletalLodModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.RegenerateSkeletalLodResult.bSuccess = false;
	Mock.RegenerateSkeletalLodResult.ErrorMessage = TEXT("Mesh not found");
	FRegenerateSkeletalLodImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Mesh not found")));
	return true;
}

// ---------------------------------------------------------------------------
// GetSkeletonInfo
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSkeletonInfoMetadataTest,
	"MCPServer.Unit.SkeletalMesh.GetSkeletonInfo.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSkeletonInfoMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FGetSkeletonInfoImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_skeleton_info"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSkeletonInfoSuccessTest,
	"MCPServer.Unit.SkeletalMesh.GetSkeletonInfo.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSkeletonInfoSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.GetSkeletonInfoResult.bSuccess = true;
	Mock.GetSkeletonInfoResult.NumBones = 65;
	Mock.GetSkeletonInfoResult.SkeletonName = TEXT("MySkeleton");
	FGetSkeletonInfoImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains 65"), MCPTestUtils::GetResultText(Result).Contains(TEXT("65")));
	TestTrue(TEXT("Contains MySkeleton"), MCPTestUtils::GetResultText(Result).Contains(TEXT("MySkeleton")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSkeletonInfoMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.GetSkeletonInfo.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSkeletonInfoMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FGetSkeletonInfoImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetSkeletonInfoModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.GetSkeletonInfo.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetSkeletonInfoModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.GetSkeletonInfoResult.bSuccess = false;
	Mock.GetSkeletonInfoResult.ErrorMessage = TEXT("No skeleton assigned");
	FGetSkeletonInfoImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_NoSkeleton"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No skeleton assigned")));
	return true;
}

// ---------------------------------------------------------------------------
// GetPhysicsAsset
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPhysicsAssetMetadataTest,
	"MCPServer.Unit.SkeletalMesh.GetPhysicsAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetPhysicsAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FGetPhysicsAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_physics_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPhysicsAssetSuccessTest,
	"MCPServer.Unit.SkeletalMesh.GetPhysicsAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetPhysicsAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.GetPhysicsAssetResult.bSuccess = true;
	Mock.GetPhysicsAssetResult.PhysicsAssetPath = TEXT("/Game/PA");
	Mock.GetPhysicsAssetResult.NumBodies = 10;
	FGetPhysicsAssetImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains /Game/PA"), MCPTestUtils::GetResultText(Result).Contains(TEXT("/Game/PA")));
	TestTrue(TEXT("Contains 10"), MCPTestUtils::GetResultText(Result).Contains(TEXT("10")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPhysicsAssetMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.GetPhysicsAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetPhysicsAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FGetPhysicsAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetPhysicsAssetModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.GetPhysicsAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetPhysicsAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.GetPhysicsAssetResult.bSuccess = false;
	Mock.GetPhysicsAssetResult.ErrorMessage = TEXT("No physics asset assigned");
	FGetPhysicsAssetImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No physics asset assigned")));
	return true;
}

// ---------------------------------------------------------------------------
// SetPhysicsAsset
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsAssetMetadataTest,
	"MCPServer.Unit.SkeletalMesh.SetPhysicsAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FSetPhysicsAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_physics_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsAssetSuccessTest,
	"MCPServer.Unit.SkeletalMesh.SetPhysicsAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.SetPhysicsAssetResult.bSuccess = true;
	FSetPhysicsAssetImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	Args->SetStringField(TEXT("physics_asset_path"), TEXT("/Game/PA_New"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains assigned"), MCPTestUtils::GetResultText(Result).Contains(TEXT("assigned")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsAssetMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.SetPhysicsAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FSetPhysicsAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsAssetModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.SetPhysicsAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.SetPhysicsAssetResult.bSuccess = false;
	Mock.SetPhysicsAssetResult.ErrorMessage = TEXT("Physics asset not found");
	FSetPhysicsAssetImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	Args->SetStringField(TEXT("physics_asset_path"), TEXT("/Game/PA_Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Physics asset not found")));
	return true;
}

// ---------------------------------------------------------------------------
// ReimportSkeletalMesh
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReimportSkeletalMeshMetadataTest,
	"MCPServer.Unit.SkeletalMesh.ReimportSkeletalMesh.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReimportSkeletalMeshMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FReimportSkeletalMeshImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("reimport_skeletal_mesh"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReimportSkeletalMeshSuccessTest,
	"MCPServer.Unit.SkeletalMesh.ReimportSkeletalMesh.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReimportSkeletalMeshSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.ReimportSkeletalMeshResult.bSuccess = true;
	FReimportSkeletalMeshImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains reimported"), MCPTestUtils::GetResultText(Result).Contains(TEXT("reimported")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReimportSkeletalMeshMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.ReimportSkeletalMesh.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReimportSkeletalMeshMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FReimportSkeletalMeshImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FReimportSkeletalMeshModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.ReimportSkeletalMesh.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FReimportSkeletalMeshModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.ReimportSkeletalMeshResult.bSuccess = false;
	Mock.ReimportSkeletalMeshResult.ErrorMessage = TEXT("Source file missing");
	FReimportSkeletalMeshImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SK_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Source file missing")));
	return true;
}

// ---------------------------------------------------------------------------
// CreateAnimAsset
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimAssetMetadataTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimAsset.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimAssetMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FCreateAnimAssetImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_anim_asset"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimAssetSuccessTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimAsset.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimAssetSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.CreateAnimAssetResult.bSuccess = true;
	Mock.CreateAnimAssetResult.AssetPath = TEXT("/Game/Anims/Walk");
	Mock.CreateAnimAssetResult.AssetName = TEXT("Walk");
	FCreateAnimAssetImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("skeleton_path"), TEXT("/Game/Skeleton"));
	Args->SetStringField(TEXT("asset_name"), TEXT("Walk"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/Anims"));
	Args->SetStringField(TEXT("asset_type"), TEXT("AnimSequence"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Walk"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Walk")));
	TestTrue(TEXT("Contains path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("/Game/Anims/Walk")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimAssetMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimAsset.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimAssetMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FCreateAnimAssetImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions skeleton_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("skeleton_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimAssetModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimAsset.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimAssetModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.CreateAnimAssetResult.bSuccess = false;
	Mock.CreateAnimAssetResult.ErrorMessage = TEXT("Invalid asset type");
	FCreateAnimAssetImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("skeleton_path"), TEXT("/Game/Skeleton"));
	Args->SetStringField(TEXT("asset_name"), TEXT("Walk"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/Anims"));
	Args->SetStringField(TEXT("asset_type"), TEXT("BadType"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid asset type")));
	return true;
}

// ---------------------------------------------------------------------------
// CreateAnimBlueprint
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimBlueprintMetadataTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimBlueprint.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimBlueprintMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FCreateAnimBlueprintImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_anim_blueprint"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimBlueprintSuccessTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimBlueprint.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimBlueprintSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.CreateAnimBlueprintResult.bSuccess = true;
	Mock.CreateAnimBlueprintResult.AssetPath = TEXT("/Game/BP/ABP_Test");
	Mock.CreateAnimBlueprintResult.AssetName = TEXT("ABP_Test");
	FCreateAnimBlueprintImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("skeleton_path"), TEXT("/Game/Skeleton"));
	Args->SetStringField(TEXT("asset_name"), TEXT("ABP_Test"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/BP"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains ABP_Test"), MCPTestUtils::GetResultText(Result).Contains(TEXT("ABP_Test")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimBlueprintMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimBlueprint.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimBlueprintMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FCreateAnimBlueprintImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions skeleton_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("skeleton_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreateAnimBlueprintModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.CreateAnimBlueprint.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreateAnimBlueprintModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.CreateAnimBlueprintResult.bSuccess = false;
	Mock.CreateAnimBlueprintResult.ErrorMessage = TEXT("Skeleton not found");
	FCreateAnimBlueprintImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("skeleton_path"), TEXT("/Game/Missing"));
	Args->SetStringField(TEXT("asset_name"), TEXT("ABP_Test"));
	Args->SetStringField(TEXT("package_path"), TEXT("/Game/BP"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Skeleton not found")));
	return true;
}

// ---------------------------------------------------------------------------
// ApplyAnimCompression
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FApplyAnimCompressionMetadataTest,
	"MCPServer.Unit.SkeletalMesh.ApplyAnimCompression.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FApplyAnimCompressionMetadataTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FApplyAnimCompressionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("apply_anim_compression"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FApplyAnimCompressionSuccessTest,
	"MCPServer.Unit.SkeletalMesh.ApplyAnimCompression.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FApplyAnimCompressionSuccessTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.ApplyAnimCompressionResult.bSuccess = true;
	FApplyAnimCompressionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("anim_sequence_path"), TEXT("/Game/Anims/Walk"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains applied"), MCPTestUtils::GetResultText(Result).Contains(TEXT("applied")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FApplyAnimCompressionMissingArgsTest,
	"MCPServer.Unit.SkeletalMesh.ApplyAnimCompression.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FApplyAnimCompressionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	FApplyAnimCompressionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions anim_sequence_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("anim_sequence_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FApplyAnimCompressionModuleFailureTest,
	"MCPServer.Unit.SkeletalMesh.ApplyAnimCompression.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FApplyAnimCompressionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockSkeletalMeshModule Mock;
	Mock.ApplyAnimCompressionResult.bSuccess = false;
	Mock.ApplyAnimCompressionResult.ErrorMessage = TEXT("Animation sequence not found");
	FApplyAnimCompressionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("anim_sequence_path"), TEXT("/Game/Anims/Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Animation sequence not found")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
