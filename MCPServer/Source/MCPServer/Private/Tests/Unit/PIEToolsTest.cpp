// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/PlayInEditorImplTool.h"
#include "Tools/Impl/StopPieImplTool.h"
#include "Tools/Impl/SimulateInEditorImplTool.h"
#include "Tools/Impl/IsPlayingImplTool.h"
#include "Tests/Mocks/MockPIEModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// PlayInEditor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPlayInEditorMetadataTest,
	"MCPServer.Unit.PIE.PlayInEditor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPlayInEditorMetadataTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	FPlayInEditorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("play_in_editor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPlayInEditorSuccessTest,
	"MCPServer.Unit.PIE.PlayInEditor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPlayInEditorSuccessTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.PlayInEditorResult.bSuccess = true;
	Mock.PlayInEditorResult.Message = TEXT("PIE started");

	FPlayInEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("PlayInEditor")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPlayInEditorEmptyArgsTest,
	"MCPServer.Unit.PIE.PlayInEditor.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPlayInEditorEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.PlayInEditorResult.bSuccess = true;

	FPlayInEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("PlayInEditor")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPlayInEditorModuleFailureTest,
	"MCPServer.Unit.PIE.PlayInEditor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPlayInEditorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.PlayInEditorResult.bSuccess = false;
	Mock.PlayInEditorResult.ErrorMessage = TEXT("Already playing");

	FPlayInEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("PlayInEditor")), 1);
	return true;
}

// ============================================================================
// StopPIE
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStopPIEMetadataTest,
	"MCPServer.Unit.PIE.StopPIE.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FStopPIEMetadataTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	FStopPieImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("stop_pie"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStopPIEEmptyArgsTest,
	"MCPServer.Unit.PIE.StopPIE.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FStopPIEEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.StopPIEResult.bSuccess = true;

	FStopPieImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("StopPIE")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStopPIEModuleFailureTest,
	"MCPServer.Unit.PIE.StopPIE.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FStopPIEModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.StopPIEResult.bSuccess = false;
	Mock.StopPIEResult.ErrorMessage = TEXT("Not currently playing");

	FStopPieImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("StopPIE")), 1);
	return true;
}

// ============================================================================
// SimulateInEditor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimulateInEditorMetadataTest,
	"MCPServer.Unit.PIE.SimulateInEditor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSimulateInEditorMetadataTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	FSimulateInEditorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("simulate_in_editor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimulateInEditorEmptyArgsTest,
	"MCPServer.Unit.PIE.SimulateInEditor.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSimulateInEditorEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.SimulateInEditorResult.bSuccess = true;

	FSimulateInEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SimulateInEditor")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSimulateInEditorModuleFailureTest,
	"MCPServer.Unit.PIE.SimulateInEditor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSimulateInEditorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.SimulateInEditorResult.bSuccess = false;
	Mock.SimulateInEditorResult.ErrorMessage = TEXT("Already simulating");

	FSimulateInEditorImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SimulateInEditor")), 1);
	return true;
}

// ============================================================================
// IsPlaying
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsPlayingMetadataTest,
	"MCPServer.Unit.PIE.IsPlaying.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FIsPlayingMetadataTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	FIsPlayingImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("is_playing"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsPlayingSuccessTest,
	"MCPServer.Unit.PIE.IsPlaying.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FIsPlayingSuccessTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.IsPlayingResult.bSuccess = true;
	Mock.IsPlayingResult.bIsPlaying = false;
	Mock.IsPlayingResult.bIsSimulating = false;

	FIsPlayingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("IsPlaying")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsPlayingEmptyArgsTest,
	"MCPServer.Unit.PIE.IsPlaying.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FIsPlayingEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.IsPlayingResult.bSuccess = true;

	FIsPlayingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("IsPlaying")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsPlayingModuleFailureTest,
	"MCPServer.Unit.PIE.IsPlaying.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FIsPlayingModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockPIEModule Mock;
	Mock.IsPlayingResult.bSuccess = false;
	Mock.IsPlayingResult.ErrorMessage = TEXT("Could not determine play state");

	FIsPlayingImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	// is_playing always returns isError=false (status report, not an error)
	TestTrue(TEXT("Always succeeds"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("IsPlaying")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
