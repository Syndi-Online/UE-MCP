// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/SetStaticMeshLodImplTool.h"
#include "Tools/Impl/ImportLodImplTool.h"
#include "Tools/Impl/GetLodSettingsImplTool.h"
#include "Tools/Impl/SetCollisionImplTool.h"
#include "Tools/Impl/AddUVChannelImplTool.h"
#include "Tools/Impl/RemoveUVChannelImplTool.h"
#include "Tools/Impl/GenerateUVChannelImplTool.h"
#include "Tools/Impl/SetMeshMaterialImplTool.h"
#include "Tools/Impl/GetMeshBoundsImplTool.h"
#include "Tests/Mocks/MockStaticMeshModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ---------------------------------------------------------------------------
// SetStaticMeshLod
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetStaticMeshLodMetadataTest,
	"MCPServer.Unit.StaticMesh.SetStaticMeshLod.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetStaticMeshLodMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FSetStaticMeshLodImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_static_mesh_lod"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetStaticMeshLodSuccessTest,
	"MCPServer.Unit.StaticMesh.SetStaticMeshLod.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetStaticMeshLodSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.SetStaticMeshLodResult.bSuccess = true;
	FSetStaticMeshLodImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	TArray<TSharedPtr<FJsonValue>> ScreenSizes;
	ScreenSizes.Add(MakeShared<FJsonValueNumber>(1.0));
	ScreenSizes.Add(MakeShared<FJsonValueNumber>(0.5));
	Args->SetArrayField(TEXT("screen_sizes"), ScreenSizes);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains LOD"), MCPTestUtils::GetResultText(Result).Contains(TEXT("LOD")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetStaticMeshLodMissingArgsTest,
	"MCPServer.Unit.StaticMesh.SetStaticMeshLod.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetStaticMeshLodMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FSetStaticMeshLodImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetStaticMeshLodModuleFailureTest,
	"MCPServer.Unit.StaticMesh.SetStaticMeshLod.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetStaticMeshLodModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.SetStaticMeshLodResult.bSuccess = false;
	Mock.SetStaticMeshLodResult.ErrorMessage = TEXT("Mesh not found");
	FSetStaticMeshLodImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Missing"));
	TArray<TSharedPtr<FJsonValue>> ScreenSizes;
	ScreenSizes.Add(MakeShared<FJsonValueNumber>(1.0));
	Args->SetArrayField(TEXT("screen_sizes"), ScreenSizes);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Mesh not found")));
	return true;
}

// ---------------------------------------------------------------------------
// ImportLod
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLodMetadataTest,
	"MCPServer.Unit.StaticMesh.ImportLod.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLodMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FImportLodImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("import_lod"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLodSuccessTest,
	"MCPServer.Unit.StaticMesh.ImportLod.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLodSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.ImportLodResult.bSuccess = true;
	Mock.ImportLodResult.LodIndex = 1;
	FImportLodImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 1);
	Args->SetStringField(TEXT("source_file"), TEXT("D:/meshes/lod1.fbx"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains LodIndex"), MCPTestUtils::GetResultText(Result).Contains(TEXT("1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLodMissingArgsTest,
	"MCPServer.Unit.StaticMesh.ImportLod.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLodMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FImportLodImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImportLodModuleFailureTest,
	"MCPServer.Unit.StaticMesh.ImportLod.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FImportLodModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.ImportLodResult.bSuccess = false;
	Mock.ImportLodResult.ErrorMessage = TEXT("File not found");
	FImportLodImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 1);
	Args->SetStringField(TEXT("source_file"), TEXT("D:/missing.fbx"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("File not found")));
	return true;
}

// ---------------------------------------------------------------------------
// GetLodSettings
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLodSettingsMetadataTest,
	"MCPServer.Unit.StaticMesh.GetLodSettings.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLodSettingsMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FGetLodSettingsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_lod_settings"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLodSettingsSuccessTest,
	"MCPServer.Unit.StaticMesh.GetLodSettings.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLodSettingsSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.GetLodSettingsResult.bSuccess = true;
	Mock.GetLodSettingsResult.LodCount = 3;
	Mock.GetLodSettingsResult.LodGroup = TEXT("SmallProp");
	FGetLodSettingsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains LodCount 3"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3")));
	TestTrue(TEXT("Contains SmallProp"), MCPTestUtils::GetResultText(Result).Contains(TEXT("SmallProp")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLodSettingsMissingArgsTest,
	"MCPServer.Unit.StaticMesh.GetLodSettings.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLodSettingsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FGetLodSettingsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetLodSettingsModuleFailureTest,
	"MCPServer.Unit.StaticMesh.GetLodSettings.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetLodSettingsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.GetLodSettingsResult.bSuccess = false;
	Mock.GetLodSettingsResult.ErrorMessage = TEXT("Mesh not found");
	FGetLodSettingsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Missing"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Mesh not found")));
	return true;
}

// ---------------------------------------------------------------------------
// SetCollision
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCollisionMetadataTest,
	"MCPServer.Unit.StaticMesh.SetCollision.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCollisionMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FSetCollisionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_collision"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCollisionSuccessTest,
	"MCPServer.Unit.StaticMesh.SetCollision.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCollisionSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.SetCollisionResult.bSuccess = true;
	FSetCollisionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetStringField(TEXT("collision_type"), TEXT("Box"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains Box"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Box")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCollisionMissingArgsTest,
	"MCPServer.Unit.StaticMesh.SetCollision.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCollisionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FSetCollisionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCollisionModuleFailureTest,
	"MCPServer.Unit.StaticMesh.SetCollision.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCollisionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.SetCollisionResult.bSuccess = false;
	Mock.SetCollisionResult.ErrorMessage = TEXT("Invalid collision type");
	FSetCollisionImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetStringField(TEXT("collision_type"), TEXT("BadType"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Invalid collision type")));
	return true;
}

// ---------------------------------------------------------------------------
// AddUVChannel
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddUVChannelMetadataTest,
	"MCPServer.Unit.StaticMesh.AddUVChannel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddUVChannelMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FAddUVChannelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_uv_channel"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddUVChannelSuccessTest,
	"MCPServer.Unit.StaticMesh.AddUVChannel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddUVChannelSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.AddUVChannelResult.bSuccess = true;
	Mock.AddUVChannelResult.NumUVChannels = 3;
	FAddUVChannelImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 0);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains NumUVChannels 3"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddUVChannelMissingArgsTest,
	"MCPServer.Unit.StaticMesh.AddUVChannel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddUVChannelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FAddUVChannelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddUVChannelModuleFailureTest,
	"MCPServer.Unit.StaticMesh.AddUVChannel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddUVChannelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.AddUVChannelResult.bSuccess = false;
	Mock.AddUVChannelResult.ErrorMessage = TEXT("Max UV channels reached");
	FAddUVChannelImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 0);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Max UV channels reached")));
	return true;
}

// ---------------------------------------------------------------------------
// RemoveUVChannel
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveUVChannelMetadataTest,
	"MCPServer.Unit.StaticMesh.RemoveUVChannel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveUVChannelMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FRemoveUVChannelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("remove_uv_channel"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveUVChannelSuccessTest,
	"MCPServer.Unit.StaticMesh.RemoveUVChannel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveUVChannelSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.RemoveUVChannelResult.bSuccess = true;
	Mock.RemoveUVChannelResult.NumUVChannels = 2;
	FRemoveUVChannelImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 0);
	Args->SetNumberField(TEXT("uv_channel_index"), 1);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains removed"), MCPTestUtils::GetResultText(Result).Contains(TEXT("removed")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveUVChannelMissingArgsTest,
	"MCPServer.Unit.StaticMesh.RemoveUVChannel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveUVChannelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FRemoveUVChannelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveUVChannelModuleFailureTest,
	"MCPServer.Unit.StaticMesh.RemoveUVChannel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveUVChannelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.RemoveUVChannelResult.bSuccess = false;
	Mock.RemoveUVChannelResult.ErrorMessage = TEXT("UV channel does not exist");
	FRemoveUVChannelImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 0);
	Args->SetNumberField(TEXT("uv_channel_index"), 99);
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("UV channel does not exist")));
	return true;
}

// ---------------------------------------------------------------------------
// GenerateUVChannel
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGenerateUVChannelMetadataTest,
	"MCPServer.Unit.StaticMesh.GenerateUVChannel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGenerateUVChannelMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FGenerateUVChannelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("generate_uv_channel"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGenerateUVChannelSuccessTest,
	"MCPServer.Unit.StaticMesh.GenerateUVChannel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGenerateUVChannelSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.GenerateUVChannelResult.bSuccess = true;
	Mock.GenerateUVChannelResult.NumUVChannels = 3;
	FGenerateUVChannelImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 0);
	Args->SetNumberField(TEXT("uv_channel_index"), 2);
	Args->SetStringField(TEXT("projection_type"), TEXT("Box"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains generated"), MCPTestUtils::GetResultText(Result).Contains(TEXT("generated")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGenerateUVChannelMissingArgsTest,
	"MCPServer.Unit.StaticMesh.GenerateUVChannel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGenerateUVChannelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FGenerateUVChannelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGenerateUVChannelModuleFailureTest,
	"MCPServer.Unit.StaticMesh.GenerateUVChannel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGenerateUVChannelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.GenerateUVChannelResult.bSuccess = false;
	Mock.GenerateUVChannelResult.ErrorMessage = TEXT("Generation failed");
	FGenerateUVChannelImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("lod_index"), 0);
	Args->SetNumberField(TEXT("uv_channel_index"), 2);
	Args->SetStringField(TEXT("projection_type"), TEXT("Box"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Generation failed")));
	return true;
}

// ---------------------------------------------------------------------------
// SetMeshMaterial
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMeshMaterialMetadataTest,
	"MCPServer.Unit.StaticMesh.SetMeshMaterial.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMeshMaterialMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FSetMeshMaterialImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_mesh_material"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMeshMaterialSuccessTest,
	"MCPServer.Unit.StaticMesh.SetMeshMaterial.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMeshMaterialSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.SetMeshMaterialResult.bSuccess = true;
	FSetMeshMaterialImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("material_index"), 0);
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_New"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains assigned"), MCPTestUtils::GetResultText(Result).Contains(TEXT("assigned")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMeshMaterialMissingArgsTest,
	"MCPServer.Unit.StaticMesh.SetMeshMaterial.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMeshMaterialMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FSetMeshMaterialImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetMeshMaterialModuleFailureTest,
	"MCPServer.Unit.StaticMesh.SetMeshMaterial.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetMeshMaterialModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.SetMeshMaterialResult.bSuccess = false;
	Mock.SetMeshMaterialResult.ErrorMessage = TEXT("Material slot out of range");
	FSetMeshMaterialImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	Args->SetNumberField(TEXT("material_index"), 99);
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/M_New"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Material slot out of range")));
	return true;
}

// ---------------------------------------------------------------------------
// GetMeshBounds
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMeshBoundsMetadataTest,
	"MCPServer.Unit.StaticMesh.GetMeshBounds.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMeshBoundsMetadataTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FGetMeshBoundsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_mesh_bounds"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMeshBoundsSuccessTest,
	"MCPServer.Unit.StaticMesh.GetMeshBounds.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMeshBoundsSuccessTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.GetMeshBoundsResult.bSuccess = true;
	Mock.GetMeshBoundsResult.SphereRadius = 50.0f;
	Mock.GetMeshBoundsResult.Origin = FVector(0, 0, 0);
	FGetMeshBoundsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Test"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains SphereRadius"), MCPTestUtils::GetResultText(Result).Contains(TEXT("50.000")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMeshBoundsMissingArgsTest,
	"MCPServer.Unit.StaticMesh.GetMeshBounds.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMeshBoundsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	FGetMeshBoundsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message mentions mesh_path"),
		MCPTestUtils::GetResultText(Result).Contains(TEXT("mesh_path")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetMeshBoundsModuleFailureTest,
	"MCPServer.Unit.StaticMesh.GetMeshBounds.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetMeshBoundsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;
	Mock.GetMeshBoundsResult.bSuccess = false;
	Mock.GetMeshBoundsResult.ErrorMessage = TEXT("Mesh has no geometry");
	FGetMeshBoundsImplTool Tool(Mock);

	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/SM_Empty"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Error message"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Mesh has no geometry")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
