// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/CreatePhysicsCollisionImplTool.h"
#include "Tools/Impl/SetPhysicalMaterialImplTool.h"
#include "Tools/Impl/SetPhysicsVelocityImplTool.h"
#include "Tools/Impl/ManageConstraintsImplTool.h"
#include "Tests/Mocks/MockPhysicsModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// CreatePhysicsCollision
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreatePhysicsCollisionMetadataTest,
	"MCPServer.Unit.Physics.CreatePhysicsCollision.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreatePhysicsCollisionMetadataTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FCreatePhysicsCollisionImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("create_physics_collision"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreatePhysicsCollisionSuccessTest,
	"MCPServer.Unit.Physics.CreatePhysicsCollision.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreatePhysicsCollisionSuccessTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.CreatePhysicsCollisionResult.bSuccess = true;
	Mock.CreatePhysicsCollisionResult.ShapesAdded = 1;

	FCreatePhysicsCollisionImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("Cube_0"));
	Args->SetStringField(TEXT("shape_type"), TEXT("box"));
	auto Dims = MakeShared<FJsonObject>();
	Dims->SetNumberField(TEXT("x"), 50.0);
	Dims->SetNumberField(TEXT("y"), 50.0);
	Dims->SetNumberField(TEXT("z"), 50.0);
	Args->SetObjectField(TEXT("dimensions"), Dims);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("CreatePhysicsCollision")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreatePhysicsCollisionMissingArgsTest,
	"MCPServer.Unit.Physics.CreatePhysicsCollision.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreatePhysicsCollisionMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FCreatePhysicsCollisionImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("CreatePhysicsCollision")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCreatePhysicsCollisionModuleFailureTest,
	"MCPServer.Unit.Physics.CreatePhysicsCollision.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FCreatePhysicsCollisionModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.CreatePhysicsCollisionResult.bSuccess = false;
	Mock.CreatePhysicsCollisionResult.ErrorMessage = TEXT("Actor not found");

	FCreatePhysicsCollisionImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("Missing"));
	Args->SetStringField(TEXT("shape_type"), TEXT("box"));
	auto Dims = MakeShared<FJsonObject>();
	Dims->SetNumberField(TEXT("x"), 50.0);
	Dims->SetNumberField(TEXT("y"), 50.0);
	Dims->SetNumberField(TEXT("z"), 50.0);
	Args->SetObjectField(TEXT("dimensions"), Dims);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("CreatePhysicsCollision")), 1);
	return true;
}

// ============================================================================
// SetPhysicalMaterial
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicalMaterialMetadataTest,
	"MCPServer.Unit.Physics.SetPhysicalMaterial.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicalMaterialMetadataTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FSetPhysicalMaterialImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_physical_material"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicalMaterialSuccessTest,
	"MCPServer.Unit.Physics.SetPhysicalMaterial.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicalMaterialSuccessTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.SetPhysicalMaterialResult.bSuccess = true;

	FSetPhysicalMaterialImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("Cube_0"));
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/Materials/PM_Metal"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetPhysicalMaterial")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicalMaterialMissingArgsTest,
	"MCPServer.Unit.Physics.SetPhysicalMaterial.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicalMaterialMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FSetPhysicalMaterialImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("SetPhysicalMaterial")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicalMaterialModuleFailureTest,
	"MCPServer.Unit.Physics.SetPhysicalMaterial.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicalMaterialModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.SetPhysicalMaterialResult.bSuccess = false;
	Mock.SetPhysicalMaterialResult.ErrorMessage = TEXT("Material not found");

	FSetPhysicalMaterialImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("Cube_0"));
	Args->SetStringField(TEXT("material_path"), TEXT("/Game/Materials/Missing"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetPhysicalMaterial")), 1);
	return true;
}

// ============================================================================
// SetPhysicsVelocity
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsVelocityMetadataTest,
	"MCPServer.Unit.Physics.SetPhysicsVelocity.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsVelocityMetadataTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FSetPhysicsVelocityImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_physics_velocity"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsVelocitySuccessTest,
	"MCPServer.Unit.Physics.SetPhysicsVelocity.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsVelocitySuccessTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.SetPhysicsVelocityResult.bSuccess = true;

	FSetPhysicsVelocityImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("Cube_0"));

	TSharedPtr<FJsonObject> LinearVel = MakeShared<FJsonObject>();
	LinearVel->SetNumberField(TEXT("x"), 100.0);
	LinearVel->SetNumberField(TEXT("y"), 0.0);
	LinearVel->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("linear_velocity"), LinearVel);

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetPhysicsVelocity")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsVelocityMissingArgsTest,
	"MCPServer.Unit.Physics.SetPhysicsVelocity.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsVelocityMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FSetPhysicsVelocityImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("SetPhysicsVelocity")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetPhysicsVelocityModuleFailureTest,
	"MCPServer.Unit.Physics.SetPhysicsVelocity.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetPhysicsVelocityModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.SetPhysicsVelocityResult.bSuccess = false;
	Mock.SetPhysicsVelocityResult.ErrorMessage = TEXT("Actor has no physics");

	FSetPhysicsVelocityImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("Cube_0"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetPhysicsVelocity")), 1);
	return true;
}

// ============================================================================
// ManageConstraints
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FManageConstraintsMetadataTest,
	"MCPServer.Unit.Physics.ManageConstraints.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FManageConstraintsMetadataTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FManageConstraintsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("manage_constraints"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FManageConstraintsSuccessTest,
	"MCPServer.Unit.Physics.ManageConstraints.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FManageConstraintsSuccessTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.ManageConstraintsResult.bSuccess = true;
	Mock.ManageConstraintsResult.ConstraintName = TEXT("PhysicsConstraint_0");

	FManageConstraintsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name_1"), TEXT("Cube_0"));
	Args->SetStringField(TEXT("actor_name_2"), TEXT("Cube_1"));
	Args->SetStringField(TEXT("constraint_type"), TEXT("hinge"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains constraint name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("PhysicsConstraint_0")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ManageConstraints")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FManageConstraintsMissingArgsTest,
	"MCPServer.Unit.Physics.ManageConstraints.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FManageConstraintsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	FManageConstraintsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("ManageConstraints")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FManageConstraintsModuleFailureTest,
	"MCPServer.Unit.Physics.ManageConstraints.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FManageConstraintsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;
	Mock.ManageConstraintsResult.bSuccess = false;
	Mock.ManageConstraintsResult.ErrorMessage = TEXT("Invalid constraint type");

	FManageConstraintsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name_1"), TEXT("Cube_0"));
	Args->SetStringField(TEXT("actor_name_2"), TEXT("Cube_1"));
	Args->SetStringField(TEXT("constraint_type"), TEXT("invalid"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ManageConstraints")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
