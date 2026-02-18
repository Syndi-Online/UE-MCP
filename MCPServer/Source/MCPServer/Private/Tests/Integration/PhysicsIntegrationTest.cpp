// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockPhysicsModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/CreatePhysicsCollisionImplTool.h"
#include "Tools/Impl/SetPhysicsVelocityImplTool.h"
#include "Tools/Impl/ManageConstraintsImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPhysicsIntegrationTest,
	"MCPServer.Integration.Physics.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPhysicsIntegrationTest::RunTest(const FString& Parameters)
{
	FMockPhysicsModule Mock;

	// Step 1: Create collision on actor A
	Mock.CreatePhysicsCollisionResult.bSuccess = true;
	Mock.CreatePhysicsCollisionResult.ShapesAdded = 1;
	FCreatePhysicsCollisionImplTool CollisionTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("ActorA"));
	Args->SetStringField(TEXT("shape_type"), TEXT("Box"));
	auto Dimensions = MakeShared<FJsonObject>();
	Dimensions->SetNumberField(TEXT("x"), 50.0);
	Dimensions->SetNumberField(TEXT("y"), 50.0);
	Dimensions->SetNumberField(TEXT("z"), 50.0);
	Args->SetObjectField(TEXT("dimensions"), Dimensions);
	TestTrue(TEXT("CreateCollision success"), MCPTestUtils::IsSuccess(CollisionTool.Execute(Args)));

	// Step 2: Set physics velocity on actor A
	Mock.SetPhysicsVelocityResult.bSuccess = true;
	FSetPhysicsVelocityImplTool VelocityTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name"), TEXT("ActorA"));
	auto LinearVel = MakeShared<FJsonObject>();
	LinearVel->SetNumberField(TEXT("x"), 100.0);
	LinearVel->SetNumberField(TEXT("y"), 0.0);
	LinearVel->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("linear_velocity"), LinearVel);
	Args->SetBoolField(TEXT("enable_physics"), true);
	TestTrue(TEXT("SetVelocity success"), MCPTestUtils::IsSuccess(VelocityTool.Execute(Args)));

	// Step 3: Manage constraints between A and B
	Mock.ManageConstraintsResult.bSuccess = true;
	Mock.ManageConstraintsResult.ConstraintName = TEXT("PhysicsConstraint_0");
	FManageConstraintsImplTool ConstraintTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_name_1"), TEXT("ActorA"));
	Args->SetStringField(TEXT("actor_name_2"), TEXT("ActorB"));
	Args->SetStringField(TEXT("constraint_type"), TEXT("Fixed"));
	Args->SetBoolField(TEXT("disable_collision"), true);
	TestTrue(TEXT("ManageConstraints success"), MCPTestUtils::IsSuccess(ConstraintTool.Execute(Args)));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 3);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
