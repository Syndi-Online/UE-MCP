// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockActorModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/SpawnActorImplTool.h"
#include "Tools/Impl/GetActorsInLevelImplTool.h"
#include "Tools/Impl/GetActorTransformImplTool.h"
#include "Tools/Impl/SetActorTransformImplTool.h"
#include "Tools/Impl/SelectActorImplTool.h"
#include "Tools/Impl/DeselectAllImplTool.h"
#include "Tools/Impl/SetActorFolderImplTool.h"
#include "Tools/Impl/DuplicateActorImplTool.h"
#include "Tools/Impl/DeleteActorImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FActorIntegrationTest,
	"MCPServer.Integration.Actor.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FActorIntegrationTest::RunTest(const FString& Parameters)
{
	FMockActorModule Mock;

	// Step 1: SpawnActor (PointLight at 100,200,300)
	Mock.SpawnActorResult.bSuccess = true;
	Mock.SpawnActorResult.ActorName = TEXT("PointLight_0");
	Mock.SpawnActorResult.ActorLabel = TEXT("PointLight");
	Mock.SpawnActorResult.ActorClass = TEXT("/Script/Engine.PointLight");
	Mock.SpawnActorResult.Location = FVector(100.0, 200.0, 300.0);
	Mock.SpawnActorResult.Rotation = FRotator::ZeroRotator;

	FSpawnActorImplTool SpawnTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_class"), TEXT("/Script/Engine.PointLight"));
	auto Location = MakeShared<FJsonObject>();
	Location->SetNumberField(TEXT("x"), 100.0);
	Location->SetNumberField(TEXT("y"), 200.0);
	Location->SetNumberField(TEXT("z"), 300.0);
	Args->SetObjectField(TEXT("location"), Location);

	auto Result = SpawnTool.Execute(Args);
	TestTrue(TEXT("SpawnActor success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("SpawnActor result contains actor name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("PointLight_0")));

	// Step 2: GetActorsInLevel
	FActorInfo ActorInfo;
	ActorInfo.ActorName = TEXT("PointLight_0");
	ActorInfo.ActorLabel = TEXT("PointLight");
	ActorInfo.ActorClass = TEXT("/Script/Engine.PointLight");
	ActorInfo.Location = FVector(100.0, 200.0, 300.0);
	Mock.GetActorsInLevelResult.bSuccess = true;
	Mock.GetActorsInLevelResult.Actors.Add(ActorInfo);

	FGetActorsInLevelImplTool GetActorsTool(Mock);
	Args = MakeShared<FJsonObject>();
	Result = GetActorsTool.Execute(Args);
	TestTrue(TEXT("GetActorsInLevel success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("GetActorsInLevel has actors"), MCPTestUtils::GetResultText(Result).Contains(TEXT("PointLight_0")));

	// Step 3: GetActorTransform
	Mock.GetActorTransformResult.bSuccess = true;
	Mock.GetActorTransformResult.Location = FVector(100.0, 200.0, 300.0);
	Mock.GetActorTransformResult.Rotation = FRotator::ZeroRotator;
	Mock.GetActorTransformResult.Scale = FVector::OneVector;

	FGetActorTransformImplTool GetTransformTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_0"));
	TestTrue(TEXT("GetActorTransform success"), MCPTestUtils::IsSuccess(GetTransformTool.Execute(Args)));

	// Step 4: SetActorTransform (new location)
	Mock.SetActorTransformResult.bSuccess = true;
	Mock.SetActorTransformResult.Location = FVector(500.0, 600.0, 700.0);
	Mock.SetActorTransformResult.Rotation = FRotator::ZeroRotator;
	Mock.SetActorTransformResult.Scale = FVector::OneVector;

	FSetActorTransformImplTool SetTransformTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_0"));
	auto NewLocation = MakeShared<FJsonObject>();
	NewLocation->SetNumberField(TEXT("x"), 500.0);
	NewLocation->SetNumberField(TEXT("y"), 600.0);
	NewLocation->SetNumberField(TEXT("z"), 700.0);
	Args->SetObjectField(TEXT("location"), NewLocation);
	TestTrue(TEXT("SetActorTransform success"), MCPTestUtils::IsSuccess(SetTransformTool.Execute(Args)));

	// Step 5: SelectActor
	Mock.SelectActorsResult.bSuccess = true;
	Mock.SelectActorsResult.AffectedCount = 1;

	FSelectActorImplTool SelectTool(Mock);
	Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> Identifiers;
	Identifiers.Add(MakeShared<FJsonValueString>(TEXT("PointLight_0")));
	Args->SetArrayField(TEXT("actor_identifiers"), Identifiers);
	TestTrue(TEXT("SelectActor success"), MCPTestUtils::IsSuccess(SelectTool.Execute(Args)));

	// Step 6: DeselectAll
	Mock.DeselectAllResult.bSuccess = true;
	Mock.DeselectAllResult.AffectedCount = 0;

	FDeselectAllImplTool DeselectTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("DeselectAll success"), MCPTestUtils::IsSuccess(DeselectTool.Execute(Args)));

	// Step 7: SetActorFolder
	Mock.SetActorFolderResult.bSuccess = true;

	FSetActorFolderImplTool FolderTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_0"));
	Args->SetStringField(TEXT("folder_path"), TEXT("TestFolder"));
	TestTrue(TEXT("SetActorFolder success"), MCPTestUtils::IsSuccess(FolderTool.Execute(Args)));

	// Step 8: DuplicateActor
	Mock.DuplicateActorResult.bSuccess = true;
	Mock.DuplicateActorResult.ActorName = TEXT("PointLight_1");
	Mock.DuplicateActorResult.ActorLabel = TEXT("PointLight2");
	Mock.DuplicateActorResult.ActorClass = TEXT("/Script/Engine.PointLight");
	Mock.DuplicateActorResult.Location = FVector(600.0, 600.0, 700.0);

	FDuplicateActorImplTool DupTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_0"));
	auto Offset = MakeShared<FJsonObject>();
	Offset->SetNumberField(TEXT("x"), 100.0);
	Offset->SetNumberField(TEXT("y"), 0.0);
	Offset->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("offset"), Offset);
	TestTrue(TEXT("DuplicateActor success"), MCPTestUtils::IsSuccess(DupTool.Execute(Args)));

	// Step 9: DeleteActor
	Mock.DeleteActorResult.bSuccess = true;
	Mock.DeleteActorResult.ActorName = TEXT("PointLight_0");

	FDeleteActorImplTool DeleteTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("PointLight_0"));
	TestTrue(TEXT("DeleteActor success"), MCPTestUtils::IsSuccess(DeleteTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("SpawnActor called"), Mock.Recorder.GetCallCount(TEXT("SpawnActor")), 1);
	TestEqual(TEXT("GetActorsInLevel called"), Mock.Recorder.GetCallCount(TEXT("GetActorsInLevel")), 1);
	TestEqual(TEXT("GetActorTransform called"), Mock.Recorder.GetCallCount(TEXT("GetActorTransform")), 1);
	TestEqual(TEXT("SetActorTransform called"), Mock.Recorder.GetCallCount(TEXT("SetActorTransform")), 1);
	TestEqual(TEXT("SelectActors called"), Mock.Recorder.GetCallCount(TEXT("SelectActors")), 1);
	TestEqual(TEXT("DeselectAll called"), Mock.Recorder.GetCallCount(TEXT("DeselectAll")), 1);
	TestEqual(TEXT("SetActorFolder called"), Mock.Recorder.GetCallCount(TEXT("SetActorFolder")), 1);
	TestEqual(TEXT("DuplicateActor called"), Mock.Recorder.GetCallCount(TEXT("DuplicateActor")), 1);
	TestEqual(TEXT("DeleteActor called"), Mock.Recorder.GetCallCount(TEXT("DeleteActor")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 9);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
