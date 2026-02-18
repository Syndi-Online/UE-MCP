// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
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
#include "Tests/Mocks/MockActorModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// SpawnActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnActorMetadataTest,
	"MCPServer.Unit.Actors.SpawnActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpawnActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSpawnActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("spawn_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnActorSuccessTest,
	"MCPServer.Unit.Actors.SpawnActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpawnActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SpawnActorResult.bSuccess = true;
	Mock.SpawnActorResult.ActorName = TEXT("TestActor");
	Mock.SpawnActorResult.ActorLabel = TEXT("TestActor");
	Mock.SpawnActorResult.ActorClass = TEXT("PointLight");
	Mock.SpawnActorResult.Location = FVector(100, 200, 300);

	FSpawnActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_class"), TEXT("/Script/Engine.PointLight"));
	auto LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 100.0);
	LocationObj->SetNumberField(TEXT("y"), 200.0);
	LocationObj->SetNumberField(TEXT("z"), 300.0);
	Args->SetObjectField(TEXT("location"), LocationObj);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains actor name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestActor")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnActorMissingArgsTest,
	"MCPServer.Unit.Actors.SpawnActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpawnActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSpawnActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SpawnActor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSpawnActorModuleFailureTest,
	"MCPServer.Unit.Actors.SpawnActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSpawnActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SpawnActorResult.bSuccess = false;
	Mock.SpawnActorResult.ErrorMessage = TEXT("Spawn failed");

	FSpawnActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_class"), TEXT("/Script/Engine.PointLight"));
	auto LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 0.0);
	LocationObj->SetNumberField(TEXT("y"), 0.0);
	LocationObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("location"), LocationObj);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Spawn failed")));
	return true;
}

// ============================================================================
// DeleteActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteActorMetadataTest,
	"MCPServer.Unit.Actors.DeleteActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FDeleteActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("delete_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteActorSuccessTest,
	"MCPServer.Unit.Actors.DeleteActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DeleteActorResult.bSuccess = true;
	Mock.DeleteActorResult.ActorName = TEXT("Deleted");

	FDeleteActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Deleted")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteActorMissingArgsTest,
	"MCPServer.Unit.Actors.DeleteActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FDeleteActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("DeleteActor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeleteActorModuleFailureTest,
	"MCPServer.Unit.Actors.DeleteActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeleteActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DeleteActorResult.bSuccess = false;
	Mock.DeleteActorResult.ErrorMessage = TEXT("Actor not found");

	FDeleteActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Actor not found")));
	return true;
}

// ============================================================================
// DuplicateActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateActorMetadataTest,
	"MCPServer.Unit.Actors.DuplicateActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FDuplicateActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("duplicate_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateActorSuccessTest,
	"MCPServer.Unit.Actors.DuplicateActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DuplicateActorResult.bSuccess = true;
	Mock.DuplicateActorResult.ActorName = TEXT("Dup");
	Mock.DuplicateActorResult.ActorLabel = TEXT("Dup");

	FDuplicateActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("OriginalActor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains dup name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Dup")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateActorMissingArgsTest,
	"MCPServer.Unit.Actors.DuplicateActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FDuplicateActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("DuplicateActor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDuplicateActorModuleFailureTest,
	"MCPServer.Unit.Actors.DuplicateActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDuplicateActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DuplicateActorResult.bSuccess = false;
	Mock.DuplicateActorResult.ErrorMessage = TEXT("Duplicate failed");

	FDuplicateActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("OriginalActor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Duplicate failed")));
	return true;
}

// ============================================================================
// GetActorsInLevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorsInLevelMetadataTest,
	"MCPServer.Unit.Actors.GetActorsInLevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorsInLevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGetActorsInLevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_actors_in_level"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorsInLevelSuccessTest,
	"MCPServer.Unit.Actors.GetActorsInLevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorsInLevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorsInLevelResult.bSuccess = true;
	FActorInfo A1;
	A1.ActorName = TEXT("Actor1");
	A1.ActorLabel = TEXT("Actor1");
	A1.ActorClass = TEXT("StaticMeshActor");
	FActorInfo A2;
	A2.ActorName = TEXT("Actor2");
	A2.ActorLabel = TEXT("Actor2");
	A2.ActorClass = TEXT("PointLight");
	Mock.GetActorsInLevelResult.Actors.Add(A1);
	Mock.GetActorsInLevelResult.Actors.Add(A2);

	FGetActorsInLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains actor1"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Actor1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorsInLevelEmptyArgsTest,
	"MCPServer.Unit.Actors.GetActorsInLevel.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorsInLevelEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorsInLevelResult.bSuccess = true;

	FGetActorsInLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorsInLevelModuleFailureTest,
	"MCPServer.Unit.Actors.GetActorsInLevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorsInLevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorsInLevelResult.bSuccess = false;
	Mock.GetActorsInLevelResult.ErrorMessage = TEXT("No world");

	FGetActorsInLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No world")));
	return true;
}

// ============================================================================
// SelectActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSelectActorMetadataTest,
	"MCPServer.Unit.Actors.SelectActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSelectActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSelectActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("select_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSelectActorSuccessTest,
	"MCPServer.Unit.Actors.SelectActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSelectActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SelectActorsResult.bSuccess = true;
	Mock.SelectActorsResult.AffectedCount = 1;

	FSelectActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("Actor1")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains count"), MCPTestUtils::GetResultText(Result).Contains(TEXT("1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSelectActorMissingArgsTest,
	"MCPServer.Unit.Actors.SelectActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSelectActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSelectActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SelectActors")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSelectActorModuleFailureTest,
	"MCPServer.Unit.Actors.SelectActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSelectActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SelectActorsResult.bSuccess = false;
	Mock.SelectActorsResult.ErrorMessage = TEXT("Selection failed");

	FSelectActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("Actor1")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Selection failed")));
	return true;
}

// ============================================================================
// DeselectAll
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeselectAllMetadataTest,
	"MCPServer.Unit.Actors.DeselectAll.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeselectAllMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FDeselectAllImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("deselect_all"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeselectAllSuccessTest,
	"MCPServer.Unit.Actors.DeselectAll.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeselectAllSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DeselectAllResult.bSuccess = true;
	Mock.DeselectAllResult.AffectedCount = 0;

	FDeselectAllImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeselectAllEmptyArgsTest,
	"MCPServer.Unit.Actors.DeselectAll.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeselectAllEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DeselectAllResult.bSuccess = true;

	FDeselectAllImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDeselectAllModuleFailureTest,
	"MCPServer.Unit.Actors.DeselectAll.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FDeselectAllModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.DeselectAllResult.bSuccess = false;
	Mock.DeselectAllResult.ErrorMessage = TEXT("Deselect failed");

	FDeselectAllImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Deselect failed")));
	return true;
}

// ============================================================================
// GetActorTransform
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorTransformMetadataTest,
	"MCPServer.Unit.Actors.GetActorTransform.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorTransformMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGetActorTransformImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_actor_transform"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorTransformSuccessTest,
	"MCPServer.Unit.Actors.GetActorTransform.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorTransformSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorTransformResult.bSuccess = true;
	Mock.GetActorTransformResult.Location = FVector(1, 2, 3);

	FGetActorTransformImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorTransformMissingArgsTest,
	"MCPServer.Unit.Actors.GetActorTransform.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorTransformMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGetActorTransformImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetActorTransform")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorTransformModuleFailureTest,
	"MCPServer.Unit.Actors.GetActorTransform.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorTransformModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorTransformResult.bSuccess = false;
	Mock.GetActorTransformResult.ErrorMessage = TEXT("Transform error");

	FGetActorTransformImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Transform error")));
	return true;
}

// ============================================================================
// SetActorTransform
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorTransformMetadataTest,
	"MCPServer.Unit.Actors.SetActorTransform.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorTransformMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSetActorTransformImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_actor_transform"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorTransformSuccessTest,
	"MCPServer.Unit.Actors.SetActorTransform.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorTransformSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorTransformResult.bSuccess = true;
	Mock.SetActorTransformResult.Location = FVector(10, 20, 30);

	FSetActorTransformImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	auto LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 10.0);
	LocationObj->SetNumberField(TEXT("y"), 20.0);
	LocationObj->SetNumberField(TEXT("z"), 30.0);
	Args->SetObjectField(TEXT("location"), LocationObj);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorTransformMissingArgsTest,
	"MCPServer.Unit.Actors.SetActorTransform.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorTransformMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSetActorTransformImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetActorTransform")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorTransformModuleFailureTest,
	"MCPServer.Unit.Actors.SetActorTransform.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorTransformModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorTransformResult.bSuccess = false;
	Mock.SetActorTransformResult.ErrorMessage = TEXT("Set transform failed");

	FSetActorTransformImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	auto LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 10.0);
	LocationObj->SetNumberField(TEXT("y"), 20.0);
	LocationObj->SetNumberField(TEXT("z"), 30.0);
	Args->SetObjectField(TEXT("location"), LocationObj);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Set transform failed")));
	return true;
}

// ============================================================================
// GetActorProperty
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorPropertyMetadataTest,
	"MCPServer.Unit.Actors.GetActorProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGetActorPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_actor_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorPropertySuccessTest,
	"MCPServer.Unit.Actors.GetActorProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorPropertyResult.bSuccess = true;
	Mock.GetActorPropertyResult.PropertyName = TEXT("Health");
	Mock.GetActorPropertyResult.PropertyValue = TEXT("100");

	FGetActorPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("property_name"), TEXT("Health"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains property value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("100")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorPropertyMissingArgsTest,
	"MCPServer.Unit.Actors.GetActorProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGetActorPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GetActorProperty")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetActorPropertyModuleFailureTest,
	"MCPServer.Unit.Actors.GetActorProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetActorPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GetActorPropertyResult.bSuccess = false;
	Mock.GetActorPropertyResult.ErrorMessage = TEXT("Property not found");

	FGetActorPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("property_name"), TEXT("Health"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Property not found")));
	return true;
}

// ============================================================================
// SetActorProperty
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorPropertyMetadataTest,
	"MCPServer.Unit.Actors.SetActorProperty.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorPropertyMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSetActorPropertyImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_actor_property"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorPropertySuccessTest,
	"MCPServer.Unit.Actors.SetActorProperty.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorPropertySuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorPropertyResult.bSuccess = true;

	FSetActorPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("property_name"), TEXT("Health"));
	Args->SetStringField(TEXT("property_value"), TEXT("100"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorPropertyMissingArgsTest,
	"MCPServer.Unit.Actors.SetActorProperty.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorPropertyMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSetActorPropertyImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetActorProperty")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorPropertyModuleFailureTest,
	"MCPServer.Unit.Actors.SetActorProperty.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorPropertyModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorPropertyResult.bSuccess = false;
	Mock.SetActorPropertyResult.ErrorMessage = TEXT("Set property failed");

	FSetActorPropertyImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("property_name"), TEXT("Health"));
	Args->SetStringField(TEXT("property_value"), TEXT("100"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Set property failed")));
	return true;
}

// ============================================================================
// ConvertActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConvertActorMetadataTest,
	"MCPServer.Unit.Actors.ConvertActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConvertActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FConvertActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("convert_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConvertActorSuccessTest,
	"MCPServer.Unit.Actors.ConvertActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConvertActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.ConvertActorResult.bSuccess = true;
	Mock.ConvertActorResult.NewActorName = TEXT("Converted");

	FConvertActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("new_class"), TEXT("/Script/Engine.PointLight"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains new name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Converted")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConvertActorMissingArgsTest,
	"MCPServer.Unit.Actors.ConvertActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConvertActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FConvertActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("ConvertActor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConvertActorModuleFailureTest,
	"MCPServer.Unit.Actors.ConvertActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConvertActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.ConvertActorResult.bSuccess = false;
	Mock.ConvertActorResult.ErrorMessage = TEXT("Convert failed");

	FConvertActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("new_class"), TEXT("/Script/Engine.PointLight"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Convert failed")));
	return true;
}

// ============================================================================
// MoveActorsToLevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveActorsToLevelMetadataTest,
	"MCPServer.Unit.Actors.MoveActorsToLevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveActorsToLevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FMoveActorsToLevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("move_actors_to_level"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveActorsToLevelSuccessTest,
	"MCPServer.Unit.Actors.MoveActorsToLevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveActorsToLevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.MoveActorsToLevelResult.bSuccess = true;
	Mock.MoveActorsToLevelResult.MovedCount = 2;

	FMoveActorsToLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A1")));
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A2")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);
	Args->SetStringField(TEXT("level_name"), TEXT("SubLevel"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains count"), MCPTestUtils::GetResultText(Result).Contains(TEXT("2")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveActorsToLevelMissingArgsTest,
	"MCPServer.Unit.Actors.MoveActorsToLevel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveActorsToLevelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FMoveActorsToLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("MoveActorsToLevel")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMoveActorsToLevelModuleFailureTest,
	"MCPServer.Unit.Actors.MoveActorsToLevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FMoveActorsToLevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.MoveActorsToLevelResult.bSuccess = false;
	Mock.MoveActorsToLevelResult.ErrorMessage = TEXT("Move failed");

	FMoveActorsToLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A1")));
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A2")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);
	Args->SetStringField(TEXT("level_name"), TEXT("SubLevel"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Move failed")));
	return true;
}

// ============================================================================
// GroupActors
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGroupActorsMetadataTest,
	"MCPServer.Unit.Actors.GroupActors.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGroupActorsMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGroupActorsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("group_actors"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGroupActorsSuccessTest,
	"MCPServer.Unit.Actors.GroupActors.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGroupActorsSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GroupActorsResult.bSuccess = true;
	Mock.GroupActorsResult.GroupName = TEXT("TestGroup");
	Mock.GroupActorsResult.ActorCount = 2;

	FGroupActorsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A1")));
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A2")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains group name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestGroup")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGroupActorsMissingArgsTest,
	"MCPServer.Unit.Actors.GroupActors.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGroupActorsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FGroupActorsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("GroupActors")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGroupActorsModuleFailureTest,
	"MCPServer.Unit.Actors.GroupActors.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGroupActorsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.GroupActorsResult.bSuccess = false;
	Mock.GroupActorsResult.ErrorMessage = TEXT("Group failed");

	FGroupActorsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A1")));
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("A2")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Group failed")));
	return true;
}

// ============================================================================
// SetActorFolder
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorFolderMetadataTest,
	"MCPServer.Unit.Actors.SetActorFolder.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorFolderMetadataTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSetActorFolderImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_actor_folder"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorFolderSuccessTest,
	"MCPServer.Unit.Actors.SetActorFolder.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorFolderSuccessTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorFolderResult.bSuccess = true;

	FSetActorFolderImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("folder_path"), TEXT("MyFolder/SubFolder"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorFolderMissingArgsTest,
	"MCPServer.Unit.Actors.SetActorFolder.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorFolderMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	FSetActorFolderImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetActorFolder")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetActorFolderModuleFailureTest,
	"MCPServer.Unit.Actors.SetActorFolder.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetActorFolderModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;
	Mock.SetActorFolderResult.bSuccess = false;
	Mock.SetActorFolderResult.ErrorMessage = TEXT("Folder set failed");

	FSetActorFolderImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("MyActor"));
	Args->SetStringField(TEXT("folder_path"), TEXT("MyFolder/SubFolder"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Folder set failed")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
