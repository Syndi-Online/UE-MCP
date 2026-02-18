// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/AddFoliageInstancesImplTool.h"
#include "Tools/Impl/RemoveFoliageInstancesImplTool.h"
#include "Tools/Impl/RegisterFoliageMeshImplTool.h"
#include "Tools/Impl/ResimulateProceduralFoliageImplTool.h"
#include "Tests/Mocks/MockFoliageModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// AddFoliageInstances
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFoliageInstancesMetadataTest,
	"MCPServer.Unit.Foliage.AddFoliageInstances.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddFoliageInstancesMetadataTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	FAddFoliageInstancesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_foliage_instances"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFoliageInstancesSuccessTest,
	"MCPServer.Unit.Foliage.AddFoliageInstances.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddFoliageInstancesSuccessTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.AddFoliageInstancesResult.bSuccess = true;
	Mock.AddFoliageInstancesResult.InstancesAdded = 5;

	FAddFoliageInstancesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/Meshes/MyTree"));

	TArray<TSharedPtr<FJsonValue>> TransformsArray;
	TSharedPtr<FJsonObject> TransformObj = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 100.0);
	LocationObj->SetNumberField(TEXT("y"), 200.0);
	LocationObj->SetNumberField(TEXT("z"), 0.0);
	TransformObj->SetObjectField(TEXT("location"), LocationObj);
	TransformsArray.Add(MakeShared<FJsonValueObject>(TransformObj));
	Args->SetArrayField(TEXT("transforms"), TransformsArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains count"), MCPTestUtils::GetResultText(Result).Contains(TEXT("5")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("AddFoliageInstances")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFoliageInstancesMissingArgsTest,
	"MCPServer.Unit.Foliage.AddFoliageInstances.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddFoliageInstancesMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	FAddFoliageInstancesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("AddFoliageInstances")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddFoliageInstancesModuleFailureTest,
	"MCPServer.Unit.Foliage.AddFoliageInstances.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddFoliageInstancesModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.AddFoliageInstancesResult.bSuccess = false;
	Mock.AddFoliageInstancesResult.ErrorMessage = TEXT("Mesh not found");

	FAddFoliageInstancesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/Meshes/Missing"));

	TArray<TSharedPtr<FJsonValue>> TransformsArray;
	TSharedPtr<FJsonObject> TransformObj = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 0.0);
	LocationObj->SetNumberField(TEXT("y"), 0.0);
	LocationObj->SetNumberField(TEXT("z"), 0.0);
	TransformObj->SetObjectField(TEXT("location"), LocationObj);
	TransformsArray.Add(MakeShared<FJsonValueObject>(TransformObj));
	Args->SetArrayField(TEXT("transforms"), TransformsArray);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("AddFoliageInstances")), 1);
	return true;
}

// ============================================================================
// RemoveFoliageInstances
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveFoliageInstancesMetadataTest,
	"MCPServer.Unit.Foliage.RemoveFoliageInstances.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveFoliageInstancesMetadataTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	FRemoveFoliageInstancesImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("remove_foliage_instances"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveFoliageInstancesSuccessTest,
	"MCPServer.Unit.Foliage.RemoveFoliageInstances.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveFoliageInstancesSuccessTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.RemoveFoliageInstancesResult.bSuccess = true;
	Mock.RemoveFoliageInstancesResult.InstancesRemoved = 3;

	FRemoveFoliageInstancesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/Meshes/MyTree"));

	TSharedPtr<FJsonObject> CenterObj = MakeShared<FJsonObject>();
	CenterObj->SetNumberField(TEXT("x"), 100.0);
	CenterObj->SetNumberField(TEXT("y"), 200.0);
	CenterObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("center"), CenterObj);
	Args->SetNumberField(TEXT("radius"), 500.0);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains count"), MCPTestUtils::GetResultText(Result).Contains(TEXT("3")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RemoveFoliageInstances")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveFoliageInstancesMissingArgsTest,
	"MCPServer.Unit.Foliage.RemoveFoliageInstances.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveFoliageInstancesMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.RemoveFoliageInstancesResult.bSuccess = true;
	Mock.RemoveFoliageInstancesResult.InstancesRemoved = 0;

	FRemoveFoliageInstancesImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// RemoveFoliageInstances treats all params as optional, so module is still called
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RemoveFoliageInstances")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveFoliageInstancesModuleFailureTest,
	"MCPServer.Unit.Foliage.RemoveFoliageInstances.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveFoliageInstancesModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.RemoveFoliageInstancesResult.bSuccess = false;
	Mock.RemoveFoliageInstancesResult.ErrorMessage = TEXT("No foliage found");

	FRemoveFoliageInstancesImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/Meshes/MyTree"));

	TSharedPtr<FJsonObject> CenterObj = MakeShared<FJsonObject>();
	CenterObj->SetNumberField(TEXT("x"), 0.0);
	CenterObj->SetNumberField(TEXT("y"), 0.0);
	CenterObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("center"), CenterObj);
	Args->SetNumberField(TEXT("radius"), 100.0);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RemoveFoliageInstances")), 1);
	return true;
}

// ============================================================================
// RegisterFoliageMesh
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterFoliageMeshMetadataTest,
	"MCPServer.Unit.Foliage.RegisterFoliageMesh.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegisterFoliageMeshMetadataTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	FRegisterFoliageMeshImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("register_foliage_mesh"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterFoliageMeshSuccessTest,
	"MCPServer.Unit.Foliage.RegisterFoliageMesh.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegisterFoliageMeshSuccessTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.RegisterFoliageMeshResult.bSuccess = true;
	Mock.RegisterFoliageMeshResult.FoliageTypeName = TEXT("CubeFoliage");

	FRegisterFoliageMeshImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/Meshes/Cube"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains foliage type"), MCPTestUtils::GetResultText(Result).Contains(TEXT("CubeFoliage")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RegisterFoliageMesh")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterFoliageMeshMissingArgsTest,
	"MCPServer.Unit.Foliage.RegisterFoliageMesh.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegisterFoliageMeshMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	FRegisterFoliageMeshImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("RegisterFoliageMesh")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegisterFoliageMeshModuleFailureTest,
	"MCPServer.Unit.Foliage.RegisterFoliageMesh.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRegisterFoliageMeshModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.RegisterFoliageMeshResult.bSuccess = false;
	Mock.RegisterFoliageMeshResult.ErrorMessage = TEXT("Invalid mesh");

	FRegisterFoliageMeshImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Game/Meshes/Invalid"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("RegisterFoliageMesh")), 1);
	return true;
}

// ============================================================================
// ResimulateProceduralFoliage
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FResimulateProceduralFoliageMetadataTest,
	"MCPServer.Unit.Foliage.ResimulateProceduralFoliage.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FResimulateProceduralFoliageMetadataTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	FResimulateProceduralFoliageImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("resimulate_procedural_foliage"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FResimulateProceduralFoliageSuccessTest,
	"MCPServer.Unit.Foliage.ResimulateProceduralFoliage.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FResimulateProceduralFoliageSuccessTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.ResimulateProceduralFoliageResult.bSuccess = true;
	Mock.ResimulateProceduralFoliageResult.VolumesProcessed = 1;

	FResimulateProceduralFoliageImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("volume_name"), TEXT("ProceduralFoliageVolume_0"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains volume count"), MCPTestUtils::GetResultText(Result).Contains(TEXT("1")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ResimulateProceduralFoliage")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FResimulateProceduralFoliageMissingArgsTest,
	"MCPServer.Unit.Foliage.ResimulateProceduralFoliage.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FResimulateProceduralFoliageMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.ResimulateProceduralFoliageResult.bSuccess = true;

	FResimulateProceduralFoliageImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// volume_name is optional, so empty args still calls the module
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ResimulateProceduralFoliage")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FResimulateProceduralFoliageModuleFailureTest,
	"MCPServer.Unit.Foliage.ResimulateProceduralFoliage.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FResimulateProceduralFoliageModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;
	Mock.ResimulateProceduralFoliageResult.bSuccess = false;
	Mock.ResimulateProceduralFoliageResult.ErrorMessage = TEXT("Volume not found");

	FResimulateProceduralFoliageImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("volume_name"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ResimulateProceduralFoliage")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
