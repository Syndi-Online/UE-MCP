// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockLevelModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/NewLevelImplTool.h"
#include "Tools/Impl/SaveLevelImplTool.h"
#include "Tools/Impl/AddSublevelImplTool.h"
#include "Tools/Impl/SetCurrentLevelImplTool.h"
#include "Tools/Impl/SetLevelVisibilityImplTool.h"
#include "Tools/Impl/RemoveSublevelImplTool.h"
#include "Tools/Impl/SaveAllDirtyLevelsImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLevelIntegrationTest,
	"MCPServer.Integration.Level.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLevelIntegrationTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;

	// Step 1: NewLevel
	Mock.NewLevelResult.bSuccess = true;
	Mock.NewLevelResult.LevelName = TEXT("TestLevel");
	Mock.NewLevelResult.LevelPath = TEXT("/Game/Maps/TestLevel");

	FNewLevelImplTool NewLevelTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/TestLevel"));
	auto Result = NewLevelTool.Execute(Args);
	TestTrue(TEXT("NewLevel success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: SaveLevel
	Mock.SaveLevelResult.bSuccess = true;
	Mock.SaveLevelResult.LevelName = TEXT("TestLevel");

	FSaveLevelImplTool SaveTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("SaveLevel success"), MCPTestUtils::IsSuccess(SaveTool.Execute(Args)));

	// Step 3: AddSublevel
	Mock.AddSublevelResult.bSuccess = true;
	Mock.AddSublevelResult.SublevelName = TEXT("TestSublevel");

	FAddSublevelImplTool AddSubTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/TestSublevel"));
	TestTrue(TEXT("AddSublevel success"), MCPTestUtils::IsSuccess(AddSubTool.Execute(Args)));

	// Step 4: SetCurrentLevel
	Mock.SetCurrentLevelResult.bSuccess = true;
	Mock.SetCurrentLevelResult.LevelName = TEXT("TestSublevel");

	FSetCurrentLevelImplTool SetCurrentTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("TestSublevel"));
	TestTrue(TEXT("SetCurrentLevel success"), MCPTestUtils::IsSuccess(SetCurrentTool.Execute(Args)));

	// Step 5: SetLevelVisibility (false)
	Mock.SetLevelVisibilityResult.bSuccess = true;
	Mock.SetLevelVisibilityResult.LevelName = TEXT("TestSublevel");
	Mock.SetLevelVisibilityResult.bVisible = false;

	FSetLevelVisibilityImplTool VisibilityTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("TestSublevel"));
	Args->SetBoolField(TEXT("visible"), false);
	TestTrue(TEXT("SetLevelVisibility(false) success"), MCPTestUtils::IsSuccess(VisibilityTool.Execute(Args)));

	// Step 6: SetLevelVisibility (true)
	Mock.SetLevelVisibilityResult.bSuccess = true;
	Mock.SetLevelVisibilityResult.bVisible = true;

	FSetLevelVisibilityImplTool VisibilityTool2(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("TestSublevel"));
	Args->SetBoolField(TEXT("visible"), true);
	TestTrue(TEXT("SetLevelVisibility(true) success"), MCPTestUtils::IsSuccess(VisibilityTool2.Execute(Args)));

	// Step 7: RemoveSublevel
	Mock.RemoveSublevelResult.bSuccess = true;
	Mock.RemoveSublevelResult.SublevelName = TEXT("TestSublevel");

	FRemoveSublevelImplTool RemoveSubTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("TestSublevel"));
	TestTrue(TEXT("RemoveSublevel success"), MCPTestUtils::IsSuccess(RemoveSubTool.Execute(Args)));

	// Step 8: SaveAllDirtyLevels
	Mock.SaveAllDirtyLevelsResult.bSuccess = true;
	Mock.SaveAllDirtyLevelsResult.SavedCount = 1;

	FSaveAllDirtyLevelsImplTool SaveAllTool(Mock);
	Args = MakeShared<FJsonObject>();
	TestTrue(TEXT("SaveAllDirtyLevels success"), MCPTestUtils::IsSuccess(SaveAllTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("NewLevel called"), Mock.Recorder.GetCallCount(TEXT("NewLevel")), 1);
	TestEqual(TEXT("SaveLevel called"), Mock.Recorder.GetCallCount(TEXT("SaveLevel")), 1);
	TestEqual(TEXT("AddSublevel called"), Mock.Recorder.GetCallCount(TEXT("AddSublevel")), 1);
	TestEqual(TEXT("SetCurrentLevel called"), Mock.Recorder.GetCallCount(TEXT("SetCurrentLevel")), 1);
	TestEqual(TEXT("SetLevelVisibility called"), Mock.Recorder.GetCallCount(TEXT("SetLevelVisibility")), 2);
	TestEqual(TEXT("RemoveSublevel called"), Mock.Recorder.GetCallCount(TEXT("RemoveSublevel")), 1);
	TestEqual(TEXT("SaveAllDirtyLevels called"), Mock.Recorder.GetCallCount(TEXT("SaveAllDirtyLevels")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 8);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
