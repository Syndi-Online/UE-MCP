// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/NewLevelImplTool.h"
#include "Tools/Impl/NewLevelFromTemplateImplTool.h"
#include "Tools/Impl/LoadLevelImplTool.h"
#include "Tools/Impl/SaveLevelImplTool.h"
#include "Tools/Impl/SaveAllDirtyLevelsImplTool.h"
#include "Tools/Impl/AddSublevelImplTool.h"
#include "Tools/Impl/RemoveSublevelImplTool.h"
#include "Tools/Impl/SetCurrentLevelImplTool.h"
#include "Tools/Impl/SetLevelVisibilityImplTool.h"
#include "Tests/Mocks/MockLevelModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// NewLevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelMetadataTest,
	"MCPServer.Unit.Level.NewLevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FNewLevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("new_level"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelSuccessTest,
	"MCPServer.Unit.Level.NewLevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.NewLevelResult.bSuccess = true;
	Mock.NewLevelResult.LevelName = TEXT("TestLevel");

	FNewLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/TestLevel"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains level name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestLevel")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelMissingArgsTest,
	"MCPServer.Unit.Level.NewLevel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FNewLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("NewLevel")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelModuleFailureTest,
	"MCPServer.Unit.Level.NewLevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.NewLevelResult.bSuccess = false;
	Mock.NewLevelResult.ErrorMessage = TEXT("Create level failed");

	FNewLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/TestLevel"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Create level failed")));
	return true;
}

// ============================================================================
// NewLevelFromTemplate
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelFromTemplateMetadataTest,
	"MCPServer.Unit.Level.NewLevelFromTemplate.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelFromTemplateMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FNewLevelFromTemplateImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("new_level_from_template"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelFromTemplateSuccessTest,
	"MCPServer.Unit.Level.NewLevelFromTemplate.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelFromTemplateSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.NewLevelFromTemplateResult.bSuccess = true;
	Mock.NewLevelFromTemplateResult.LevelName = TEXT("TemplatedLevel");

	FNewLevelFromTemplateImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/TemplatedLevel"));
	Args->SetStringField(TEXT("template_path"), TEXT("/Game/Maps/Template"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains level name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TemplatedLevel")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelFromTemplateMissingArgsTest,
	"MCPServer.Unit.Level.NewLevelFromTemplate.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelFromTemplateMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FNewLevelFromTemplateImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("NewLevelFromTemplate")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNewLevelFromTemplateModuleFailureTest,
	"MCPServer.Unit.Level.NewLevelFromTemplate.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FNewLevelFromTemplateModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.NewLevelFromTemplateResult.bSuccess = false;
	Mock.NewLevelFromTemplateResult.ErrorMessage = TEXT("Template not found");

	FNewLevelFromTemplateImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/TemplatedLevel"));
	Args->SetStringField(TEXT("template_path"), TEXT("/Game/Maps/Template"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Template not found")));
	return true;
}

// ============================================================================
// LoadLevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadLevelMetadataTest,
	"MCPServer.Unit.Level.LoadLevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadLevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FLoadLevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("load_level"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadLevelSuccessTest,
	"MCPServer.Unit.Level.LoadLevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadLevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.LoadLevelResult.bSuccess = true;
	Mock.LoadLevelResult.LevelName = TEXT("LoadedLevel");

	FLoadLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/LoadedLevel"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains level name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("LoadedLevel")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadLevelMissingArgsTest,
	"MCPServer.Unit.Level.LoadLevel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadLevelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FLoadLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("LoadLevel")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLoadLevelModuleFailureTest,
	"MCPServer.Unit.Level.LoadLevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FLoadLevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.LoadLevelResult.bSuccess = false;
	Mock.LoadLevelResult.ErrorMessage = TEXT("Level not found");

	FLoadLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Level not found")));
	return true;
}

// ============================================================================
// SaveLevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveLevelMetadataTest,
	"MCPServer.Unit.Level.SaveLevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveLevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FSaveLevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("save_level"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveLevelSuccessTest,
	"MCPServer.Unit.Level.SaveLevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveLevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SaveLevelResult.bSuccess = true;
	Mock.SaveLevelResult.LevelName = TEXT("Saved");

	FSaveLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains level name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Saved")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveLevelEmptyArgsTest,
	"MCPServer.Unit.Level.SaveLevel.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveLevelEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SaveLevelResult.bSuccess = true;

	FSaveLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveLevelModuleFailureTest,
	"MCPServer.Unit.Level.SaveLevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveLevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SaveLevelResult.bSuccess = false;
	Mock.SaveLevelResult.ErrorMessage = TEXT("Save failed");

	FSaveLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Save failed")));
	return true;
}

// ============================================================================
// SaveAllDirtyLevels
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAllDirtyLevelsMetadataTest,
	"MCPServer.Unit.Level.SaveAllDirtyLevels.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAllDirtyLevelsMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FSaveAllDirtyLevelsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("save_all_dirty_levels"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAllDirtyLevelsSuccessTest,
	"MCPServer.Unit.Level.SaveAllDirtyLevels.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAllDirtyLevelsSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SaveAllDirtyLevelsResult.bSuccess = true;
	Mock.SaveAllDirtyLevelsResult.SavedCount = 3;

	FSaveAllDirtyLevelsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains saved"), MCPTestUtils::GetResultText(Result).Contains(TEXT("saved")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAllDirtyLevelsEmptyArgsTest,
	"MCPServer.Unit.Level.SaveAllDirtyLevels.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAllDirtyLevelsEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SaveAllDirtyLevelsResult.bSuccess = true;

	FSaveAllDirtyLevelsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSaveAllDirtyLevelsModuleFailureTest,
	"MCPServer.Unit.Level.SaveAllDirtyLevels.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSaveAllDirtyLevelsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SaveAllDirtyLevelsResult.bSuccess = false;
	Mock.SaveAllDirtyLevelsResult.ErrorMessage = TEXT("Save all failed");

	FSaveAllDirtyLevelsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Save all failed")));
	return true;
}

// ============================================================================
// AddSublevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddSublevelMetadataTest,
	"MCPServer.Unit.Level.AddSublevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddSublevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FAddSublevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("add_sublevel"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddSublevelSuccessTest,
	"MCPServer.Unit.Level.AddSublevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddSublevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.AddSublevelResult.bSuccess = true;
	Mock.AddSublevelResult.SublevelName = TEXT("Sub1");

	FAddSublevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/Sub1"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains sublevel name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Sub1")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddSublevelMissingArgsTest,
	"MCPServer.Unit.Level.AddSublevel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddSublevelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FAddSublevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("AddSublevel")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAddSublevelModuleFailureTest,
	"MCPServer.Unit.Level.AddSublevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FAddSublevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.AddSublevelResult.bSuccess = false;
	Mock.AddSublevelResult.ErrorMessage = TEXT("Add sublevel failed");

	FAddSublevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_path"), TEXT("/Game/Maps/Sub1"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Add sublevel failed")));
	return true;
}

// ============================================================================
// RemoveSublevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveSublevelMetadataTest,
	"MCPServer.Unit.Level.RemoveSublevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveSublevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FRemoveSublevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("remove_sublevel"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveSublevelSuccessTest,
	"MCPServer.Unit.Level.RemoveSublevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveSublevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.RemoveSublevelResult.bSuccess = true;
	Mock.RemoveSublevelResult.SublevelName = TEXT("RemovedSub");

	FRemoveSublevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("RemovedSub"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveSublevelMissingArgsTest,
	"MCPServer.Unit.Level.RemoveSublevel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveSublevelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FRemoveSublevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("RemoveSublevel")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRemoveSublevelModuleFailureTest,
	"MCPServer.Unit.Level.RemoveSublevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FRemoveSublevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.RemoveSublevelResult.bSuccess = false;
	Mock.RemoveSublevelResult.ErrorMessage = TEXT("Remove sublevel failed");

	FRemoveSublevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Remove sublevel failed")));
	return true;
}

// ============================================================================
// SetCurrentLevel
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCurrentLevelMetadataTest,
	"MCPServer.Unit.Level.SetCurrentLevel.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCurrentLevelMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FSetCurrentLevelImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_current_level"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCurrentLevelSuccessTest,
	"MCPServer.Unit.Level.SetCurrentLevel.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCurrentLevelSuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SetCurrentLevelResult.bSuccess = true;
	Mock.SetCurrentLevelResult.LevelName = TEXT("CurrentLevel");

	FSetCurrentLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("CurrentLevel"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCurrentLevelMissingArgsTest,
	"MCPServer.Unit.Level.SetCurrentLevel.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCurrentLevelMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FSetCurrentLevelImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetCurrentLevel")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCurrentLevelModuleFailureTest,
	"MCPServer.Unit.Level.SetCurrentLevel.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCurrentLevelModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SetCurrentLevelResult.bSuccess = false;
	Mock.SetCurrentLevelResult.ErrorMessage = TEXT("Set current level failed");

	FSetCurrentLevelImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Set current level failed")));
	return true;
}

// ============================================================================
// SetLevelVisibility
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLevelVisibilityMetadataTest,
	"MCPServer.Unit.Level.SetLevelVisibility.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLevelVisibilityMetadataTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FSetLevelVisibilityImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_level_visibility"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLevelVisibilitySuccessTest,
	"MCPServer.Unit.Level.SetLevelVisibility.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLevelVisibilitySuccessTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SetLevelVisibilityResult.bSuccess = true;
	Mock.SetLevelVisibilityResult.LevelName = TEXT("SubLevel");
	Mock.SetLevelVisibilityResult.bVisible = true;

	FSetLevelVisibilityImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("SubLevel"));
	Args->SetBoolField(TEXT("visible"), true);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLevelVisibilityMissingArgsTest,
	"MCPServer.Unit.Level.SetLevelVisibility.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLevelVisibilityMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	FSetLevelVisibilityImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetLevelVisibility")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetLevelVisibilityModuleFailureTest,
	"MCPServer.Unit.Level.SetLevelVisibility.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetLevelVisibilityModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockLevelModule Mock;
	Mock.SetLevelVisibilityResult.bSuccess = false;
	Mock.SetLevelVisibilityResult.ErrorMessage = TEXT("Visibility failed");

	FSetLevelVisibilityImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("level_name"), TEXT("SubLevel"));
	Args->SetBoolField(TEXT("visible"), true);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Visibility failed")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
