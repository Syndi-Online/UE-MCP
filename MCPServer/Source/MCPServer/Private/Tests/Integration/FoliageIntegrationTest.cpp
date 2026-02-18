// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockFoliageModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/RegisterFoliageMeshImplTool.h"
#include "Tools/Impl/AddFoliageInstancesImplTool.h"
#include "Tools/Impl/RemoveFoliageInstancesImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFoliageIntegrationTest,
	"MCPServer.Integration.Foliage.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFoliageIntegrationTest::RunTest(const FString& Parameters)
{
	FMockFoliageModule Mock;

	// Step 1: Register mesh
	Mock.RegisterFoliageMeshResult.bSuccess = true;
	Mock.RegisterFoliageMeshResult.FoliageTypeName = TEXT("CubeFoliage");
	FRegisterFoliageMeshImplTool RegisterTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Engine/BasicShapes/Cube"));
	TestTrue(TEXT("RegisterMesh success"), MCPTestUtils::IsSuccess(RegisterTool.Execute(Args)));

	// Step 2: Add instances
	Mock.AddFoliageInstancesResult.bSuccess = true;
	Mock.AddFoliageInstancesResult.InstancesAdded = 3;
	FAddFoliageInstancesImplTool AddTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Engine/BasicShapes/Cube"));
	TArray<TSharedPtr<FJsonValue>> TransformsArray;
	for (int32 i = 0; i < 3; ++i)
	{
		auto Transform = MakeShared<FJsonObject>();
		auto Loc = MakeShared<FJsonObject>();
		Loc->SetNumberField(TEXT("x"), i * 100.0);
		Loc->SetNumberField(TEXT("y"), 0.0);
		Loc->SetNumberField(TEXT("z"), 0.0);
		Transform->SetObjectField(TEXT("location"), Loc);
		TransformsArray.Add(MakeShared<FJsonValueObject>(Transform));
	}
	Args->SetArrayField(TEXT("transforms"), TransformsArray);
	TestTrue(TEXT("AddInstances success"), MCPTestUtils::IsSuccess(AddTool.Execute(Args)));

	// Step 3: Remove instances
	Mock.RemoveFoliageInstancesResult.bSuccess = true;
	Mock.RemoveFoliageInstancesResult.InstancesRemoved = 2;
	FRemoveFoliageInstancesImplTool RemoveTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mesh_path"), TEXT("/Engine/BasicShapes/Cube"));
	auto Center = MakeShared<FJsonObject>();
	Center->SetNumberField(TEXT("x"), 100.0);
	Center->SetNumberField(TEXT("y"), 0.0);
	Center->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("center"), Center);
	Args->SetNumberField(TEXT("radius"), 500.0);
	TestTrue(TEXT("RemoveInstances success"), MCPTestUtils::IsSuccess(RemoveTool.Execute(Args)));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 3);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
