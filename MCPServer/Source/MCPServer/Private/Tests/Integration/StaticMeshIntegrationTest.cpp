// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockStaticMeshModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/GetLodSettingsImplTool.h"
#include "Tools/Impl/GetMeshBoundsImplTool.h"
#include "Tools/Impl/SetCollisionImplTool.h"
#include "Tools/Impl/SetMeshMaterialImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStaticMeshIntegrationTest,
	"MCPServer.Integration.StaticMesh.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FStaticMeshIntegrationTest::RunTest(const FString& Parameters)
{
	FMockStaticMeshModule Mock;

	const FString MeshPath = TEXT("/Game/Meshes/SM_TestCube");

	// Step 1: GetLodSettings
	FLodInfo Lod0;
	Lod0.LodIndex = 0;
	Lod0.ScreenSize = 1.0f;
	Lod0.NumVertices = 24;
	Lod0.NumTriangles = 12;
	Mock.GetLodSettingsResult.bSuccess = true;
	Mock.GetLodSettingsResult.LodCount = 1;
	Mock.GetLodSettingsResult.LodGroup = TEXT("LargeWorld");
	Mock.GetLodSettingsResult.Lods.Add(Lod0);

	FGetLodSettingsImplTool LodTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), MeshPath);
	auto Result = LodTool.Execute(Args);
	TestTrue(TEXT("GetLodSettings success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: GetMeshBounds
	Mock.GetMeshBoundsResult.bSuccess = true;
	Mock.GetMeshBoundsResult.Origin = FVector(0.0, 0.0, 0.0);
	Mock.GetMeshBoundsResult.BoxExtent = FVector(50.0, 50.0, 50.0);
	Mock.GetMeshBoundsResult.SphereRadius = 86.6f;

	FGetMeshBoundsImplTool BoundsTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), MeshPath);
	Result = BoundsTool.Execute(Args);
	TestTrue(TEXT("GetMeshBounds success"), MCPTestUtils::IsSuccess(Result));

	// Step 3: SetCollision
	Mock.SetCollisionResult.bSuccess = true;

	FSetCollisionImplTool CollisionTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), MeshPath);
	Args->SetStringField(TEXT("collision_type"), TEXT("UseComplexAsSimple"));
	TestTrue(TEXT("SetCollision success"), MCPTestUtils::IsSuccess(CollisionTool.Execute(Args)));

	// Step 4: SetMeshMaterial
	Mock.SetMeshMaterialResult.bSuccess = true;

	FSetMeshMaterialImplTool MatTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), MeshPath);
	Args->SetNumberField(TEXT("material_index"), 0);
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/Materials/M_TestMaterial"));
	TestTrue(TEXT("SetMeshMaterial success"), MCPTestUtils::IsSuccess(MatTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("GetLodSettings called"), Mock.Recorder.GetCallCount(TEXT("GetLodSettings")), 1);
	TestEqual(TEXT("GetMeshBounds called"), Mock.Recorder.GetCallCount(TEXT("GetMeshBounds")), 1);
	TestEqual(TEXT("SetCollision called"), Mock.Recorder.GetCallCount(TEXT("SetCollision")), 1);
	TestEqual(TEXT("SetMeshMaterial called"), Mock.Recorder.GetCallCount(TEXT("SetMeshMaterial")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 4);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
