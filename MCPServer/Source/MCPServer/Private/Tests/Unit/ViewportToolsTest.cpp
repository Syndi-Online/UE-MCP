// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/GetViewportCameraImplTool.h"
#include "Tools/Impl/SetViewportCameraImplTool.h"
#include "Tools/Impl/SetGameViewImplTool.h"
#include "Tools/Impl/SetViewportRealtimeImplTool.h"
#include "Tools/Impl/SetViewModeImplTool.h"
#include "Tools/Impl/SetCameraFovImplTool.h"
#include "Tools/Impl/FocusOnActorsImplTool.h"
#include "Tools/Impl/TakeScreenshotImplTool.h"
#include "Tools/Impl/PilotActorImplTool.h"
#include "Tools/Impl/EjectPilotImplTool.h"
#include "Tools/Impl/SetBookmarkImplTool.h"
#include "Tools/Impl/JumpToBookmarkImplTool.h"
#include "Tests/Mocks/MockViewportModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// GetViewportCamera
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetViewportCameraMetadataTest,
	"MCPServer.Unit.Viewport.GetViewportCamera.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetViewportCameraMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FGetViewportCameraImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_viewport_camera"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetViewportCameraSuccessTest,
	"MCPServer.Unit.Viewport.GetViewportCamera.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetViewportCameraSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.GetViewportCameraResult.bSuccess = true;
	Mock.GetViewportCameraResult.Location = FVector(1, 2, 3);
	Mock.GetViewportCameraResult.FOV = 90.0f;

	FGetViewportCameraImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetViewportCameraEmptyArgsTest,
	"MCPServer.Unit.Viewport.GetViewportCamera.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetViewportCameraEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.GetViewportCameraResult.bSuccess = true;

	FGetViewportCameraImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetViewportCameraModuleFailureTest,
	"MCPServer.Unit.Viewport.GetViewportCamera.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetViewportCameraModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.GetViewportCameraResult.bSuccess = false;
	Mock.GetViewportCameraResult.ErrorMessage = TEXT("No viewport");

	FGetViewportCameraImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("No viewport")));
	return true;
}

// ============================================================================
// SetViewportCamera
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportCameraMetadataTest,
	"MCPServer.Unit.Viewport.SetViewportCamera.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportCameraMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetViewportCameraImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_viewport_camera"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportCameraSuccessTest,
	"MCPServer.Unit.Viewport.SetViewportCamera.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportCameraSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetViewportCameraResult.bSuccess = true;
	Mock.SetViewportCameraResult.Location = FVector(100, 200, 300);

	FSetViewportCameraImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	auto LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 100.0);
	LocationObj->SetNumberField(TEXT("y"), 200.0);
	LocationObj->SetNumberField(TEXT("z"), 300.0);
	Args->SetObjectField(TEXT("location"), LocationObj);
	auto RotationObj = MakeShared<FJsonObject>();
	RotationObj->SetNumberField(TEXT("pitch"), 0.0);
	RotationObj->SetNumberField(TEXT("yaw"), 90.0);
	RotationObj->SetNumberField(TEXT("roll"), 0.0);
	Args->SetObjectField(TEXT("rotation"), RotationObj);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportCameraMissingArgsTest,
	"MCPServer.Unit.Viewport.SetViewportCamera.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportCameraMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetViewportCameraImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetViewportCamera")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportCameraModuleFailureTest,
	"MCPServer.Unit.Viewport.SetViewportCamera.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportCameraModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetViewportCameraResult.bSuccess = false;
	Mock.SetViewportCameraResult.ErrorMessage = TEXT("Camera set failed");

	FSetViewportCameraImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	auto LocationObj = MakeShared<FJsonObject>();
	LocationObj->SetNumberField(TEXT("x"), 0.0);
	LocationObj->SetNumberField(TEXT("y"), 0.0);
	LocationObj->SetNumberField(TEXT("z"), 0.0);
	Args->SetObjectField(TEXT("location"), LocationObj);
	auto RotationObj = MakeShared<FJsonObject>();
	RotationObj->SetNumberField(TEXT("pitch"), 0.0);
	RotationObj->SetNumberField(TEXT("yaw"), 0.0);
	RotationObj->SetNumberField(TEXT("roll"), 0.0);
	Args->SetObjectField(TEXT("rotation"), RotationObj);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Camera set failed")));
	return true;
}

// ============================================================================
// SetGameView
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetGameViewMetadataTest,
	"MCPServer.Unit.Viewport.SetGameView.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetGameViewMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetGameViewImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_game_view"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetGameViewSuccessTest,
	"MCPServer.Unit.Viewport.SetGameView.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetGameViewSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetGameViewResult.bSuccess = true;

	FSetGameViewImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetBoolField(TEXT("enabled"), true);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetGameViewMissingArgsTest,
	"MCPServer.Unit.Viewport.SetGameView.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetGameViewMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetGameViewImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetGameView")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetGameViewModuleFailureTest,
	"MCPServer.Unit.Viewport.SetGameView.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetGameViewModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetGameViewResult.bSuccess = false;
	Mock.SetGameViewResult.ErrorMessage = TEXT("Game view failed");

	FSetGameViewImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetBoolField(TEXT("enabled"), true);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Game view failed")));
	return true;
}

// ============================================================================
// SetViewportRealtime
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportRealtimeMetadataTest,
	"MCPServer.Unit.Viewport.SetViewportRealtime.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportRealtimeMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetViewportRealtimeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_viewport_realtime"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportRealtimeSuccessTest,
	"MCPServer.Unit.Viewport.SetViewportRealtime.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportRealtimeSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetViewportRealtimeResult.bSuccess = true;

	FSetViewportRealtimeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetBoolField(TEXT("enabled"), true);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportRealtimeMissingArgsTest,
	"MCPServer.Unit.Viewport.SetViewportRealtime.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportRealtimeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetViewportRealtimeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetViewportRealtime")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewportRealtimeModuleFailureTest,
	"MCPServer.Unit.Viewport.SetViewportRealtime.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewportRealtimeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetViewportRealtimeResult.bSuccess = false;
	Mock.SetViewportRealtimeResult.ErrorMessage = TEXT("Realtime failed");

	FSetViewportRealtimeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetBoolField(TEXT("enabled"), true);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Realtime failed")));
	return true;
}

// ============================================================================
// SetViewMode
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewModeMetadataTest,
	"MCPServer.Unit.Viewport.SetViewMode.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewModeMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetViewModeImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_view_mode"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewModeSuccessTest,
	"MCPServer.Unit.Viewport.SetViewMode.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewModeSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetViewModeResult.bSuccess = true;

	FSetViewModeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mode"), TEXT("Wireframe"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewModeMissingArgsTest,
	"MCPServer.Unit.Viewport.SetViewMode.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewModeMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetViewModeImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetViewMode")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetViewModeModuleFailureTest,
	"MCPServer.Unit.Viewport.SetViewMode.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetViewModeModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetViewModeResult.bSuccess = false;
	Mock.SetViewModeResult.ErrorMessage = TEXT("View mode failed");

	FSetViewModeImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mode"), TEXT("Invalid"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("View mode failed")));
	return true;
}

// ============================================================================
// SetCameraFov
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCameraFovMetadataTest,
	"MCPServer.Unit.Viewport.SetCameraFov.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCameraFovMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetCameraFovImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_camera_fov"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCameraFovSuccessTest,
	"MCPServer.Unit.Viewport.SetCameraFov.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCameraFovSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetCameraFOVResult.bSuccess = true;

	FSetCameraFovImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("fov"), 90.0);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCameraFovMissingArgsTest,
	"MCPServer.Unit.Viewport.SetCameraFov.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCameraFovMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetCameraFovImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetCameraFOV")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCameraFovModuleFailureTest,
	"MCPServer.Unit.Viewport.SetCameraFov.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCameraFovModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetCameraFOVResult.bSuccess = false;
	Mock.SetCameraFOVResult.ErrorMessage = TEXT("FOV failed");

	FSetCameraFovImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("fov"), 90.0);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("FOV failed")));
	return true;
}

// ============================================================================
// FocusOnActors
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFocusOnActorsMetadataTest,
	"MCPServer.Unit.Viewport.FocusOnActors.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFocusOnActorsMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FFocusOnActorsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("focus_on_actors"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFocusOnActorsSuccessTest,
	"MCPServer.Unit.Viewport.FocusOnActors.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFocusOnActorsSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.FocusOnActorsResult.bSuccess = true;

	FFocusOnActorsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("Actor1")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFocusOnActorsMissingArgsTest,
	"MCPServer.Unit.Viewport.FocusOnActors.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFocusOnActorsMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FFocusOnActorsImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("FocusOnActors")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFocusOnActorsModuleFailureTest,
	"MCPServer.Unit.Viewport.FocusOnActors.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FFocusOnActorsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.FocusOnActorsResult.bSuccess = false;
	Mock.FocusOnActorsResult.ErrorMessage = TEXT("Focus failed");

	FFocusOnActorsImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> IdentifiersArray;
	IdentifiersArray.Add(MakeShared<FJsonValueString>(TEXT("Actor1")));
	Args->SetArrayField(TEXT("actor_identifiers"), IdentifiersArray);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Focus failed")));
	return true;
}

// ============================================================================
// TakeScreenshot
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTakeScreenshotMetadataTest,
	"MCPServer.Unit.Viewport.TakeScreenshot.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FTakeScreenshotMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FTakeScreenshotImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("take_screenshot"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTakeScreenshotSuccessTest,
	"MCPServer.Unit.Viewport.TakeScreenshot.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FTakeScreenshotSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.TakeScreenshotResult.bSuccess = true;
	Mock.TakeScreenshotResult.FilePath = TEXT("/tmp/shot.png");

	FTakeScreenshotImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("filename"), TEXT("shot.png"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains path"), MCPTestUtils::GetResultText(Result).Contains(TEXT("shot.png")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTakeScreenshotMissingArgsTest,
	"MCPServer.Unit.Viewport.TakeScreenshot.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FTakeScreenshotMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.TakeScreenshotResult.bSuccess = true;

	FTakeScreenshotImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	// filename is optional, so empty args still calls the module
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Mock called"), Mock.Recorder.GetCallCount(TEXT("TakeScreenshot")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTakeScreenshotModuleFailureTest,
	"MCPServer.Unit.Viewport.TakeScreenshot.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FTakeScreenshotModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.TakeScreenshotResult.bSuccess = false;
	Mock.TakeScreenshotResult.ErrorMessage = TEXT("Screenshot failed");

	FTakeScreenshotImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("filename"), TEXT("shot.png"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Screenshot failed")));
	return true;
}

// ============================================================================
// PilotActor
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPilotActorMetadataTest,
	"MCPServer.Unit.Viewport.PilotActor.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPilotActorMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FPilotActorImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("pilot_actor"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPilotActorSuccessTest,
	"MCPServer.Unit.Viewport.PilotActor.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPilotActorSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.PilotActorResult.bSuccess = true;

	FPilotActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("CameraActor_0"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPilotActorMissingArgsTest,
	"MCPServer.Unit.Viewport.PilotActor.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPilotActorMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FPilotActorImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("PilotActor")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPilotActorModuleFailureTest,
	"MCPServer.Unit.Viewport.PilotActor.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FPilotActorModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.PilotActorResult.bSuccess = false;
	Mock.PilotActorResult.ErrorMessage = TEXT("Pilot failed");

	FPilotActorImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("actor_identifier"), TEXT("CameraActor_0"));

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Pilot failed")));
	return true;
}

// ============================================================================
// EjectPilot
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEjectPilotMetadataTest,
	"MCPServer.Unit.Viewport.EjectPilot.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FEjectPilotMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FEjectPilotImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("eject_pilot"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEjectPilotSuccessTest,
	"MCPServer.Unit.Viewport.EjectPilot.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FEjectPilotSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.EjectPilotResult.bSuccess = true;

	FEjectPilotImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEjectPilotEmptyArgsTest,
	"MCPServer.Unit.Viewport.EjectPilot.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FEjectPilotEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.EjectPilotResult.bSuccess = true;

	FEjectPilotImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEjectPilotModuleFailureTest,
	"MCPServer.Unit.Viewport.EjectPilot.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FEjectPilotModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.EjectPilotResult.bSuccess = false;
	Mock.EjectPilotResult.ErrorMessage = TEXT("Eject failed");

	FEjectPilotImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Eject failed")));
	return true;
}

// ============================================================================
// SetBookmark
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBookmarkMetadataTest,
	"MCPServer.Unit.Viewport.SetBookmark.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBookmarkMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetBookmarkImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_bookmark"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBookmarkSuccessTest,
	"MCPServer.Unit.Viewport.SetBookmark.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBookmarkSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetBookmarkResult.bSuccess = true;

	FSetBookmarkImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("index"), 0);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBookmarkMissingArgsTest,
	"MCPServer.Unit.Viewport.SetBookmark.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBookmarkMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FSetBookmarkImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("SetBookmark")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetBookmarkModuleFailureTest,
	"MCPServer.Unit.Viewport.SetBookmark.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetBookmarkModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.SetBookmarkResult.bSuccess = false;
	Mock.SetBookmarkResult.ErrorMessage = TEXT("Bookmark failed");

	FSetBookmarkImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("index"), 0);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Bookmark failed")));
	return true;
}

// ============================================================================
// JumpToBookmark
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJumpToBookmarkMetadataTest,
	"MCPServer.Unit.Viewport.JumpToBookmark.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJumpToBookmarkMetadataTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FJumpToBookmarkImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("jump_to_bookmark"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJumpToBookmarkSuccessTest,
	"MCPServer.Unit.Viewport.JumpToBookmark.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJumpToBookmarkSuccessTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.JumpToBookmarkResult.bSuccess = true;

	FJumpToBookmarkImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("index"), 0);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJumpToBookmarkMissingArgsTest,
	"MCPServer.Unit.Viewport.JumpToBookmark.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJumpToBookmarkMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	FJumpToBookmarkImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Mock not called"), Mock.Recorder.GetCallCount(TEXT("JumpToBookmark")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJumpToBookmarkModuleFailureTest,
	"MCPServer.Unit.Viewport.JumpToBookmark.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJumpToBookmarkModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;
	Mock.JumpToBookmarkResult.bSuccess = false;
	Mock.JumpToBookmarkResult.ErrorMessage = TEXT("Jump failed");

	FJumpToBookmarkImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("index"), 0);

	auto Result = Tool.Execute(Args);
	TestTrue(TEXT("IsError"), MCPTestUtils::IsError(Result));
	TestTrue(TEXT("Contains error"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Jump failed")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
