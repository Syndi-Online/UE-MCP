// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockViewportModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/GetViewportCameraImplTool.h"
#include "Tools/Impl/SetViewportCameraImplTool.h"
#include "Tools/Impl/SetCameraFovImplTool.h"
#include "Tools/Impl/SetGameViewImplTool.h"
#include "Tools/Impl/SetViewModeImplTool.h"
#include "Tools/Impl/SetBookmarkImplTool.h"
#include "Tools/Impl/JumpToBookmarkImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FViewportIntegrationTest,
	"MCPServer.Integration.Viewport.CameraWorkflow",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FViewportIntegrationTest::RunTest(const FString& Parameters)
{
	FMockViewportModule Mock;

	// Step 1: GetViewportCamera (initial state)
	Mock.GetViewportCameraResult.bSuccess = true;
	Mock.GetViewportCameraResult.Location = FVector(0.0, 0.0, 500.0);
	Mock.GetViewportCameraResult.Rotation = FRotator(-45.0, 0.0, 0.0);
	Mock.GetViewportCameraResult.FOV = 90.0f;

	FGetViewportCameraImplTool GetCamTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	auto Result = GetCamTool.Execute(Args);
	TestTrue(TEXT("GetViewportCamera success"), MCPTestUtils::IsSuccess(Result));

	// Step 2: SetViewportCamera (new position)
	Mock.SetViewportCameraResult.bSuccess = true;
	Mock.SetViewportCameraResult.Location = FVector(1000.0, 2000.0, 500.0);
	Mock.SetViewportCameraResult.Rotation = FRotator(-30.0, 45.0, 0.0);

	FSetViewportCameraImplTool SetCamTool(Mock);
	Args = MakeShared<FJsonObject>();
	auto Location = MakeShared<FJsonObject>();
	Location->SetNumberField(TEXT("x"), 1000.0);
	Location->SetNumberField(TEXT("y"), 2000.0);
	Location->SetNumberField(TEXT("z"), 500.0);
	Args->SetObjectField(TEXT("location"), Location);
	auto Rotation = MakeShared<FJsonObject>();
	Rotation->SetNumberField(TEXT("pitch"), -30.0);
	Rotation->SetNumberField(TEXT("yaw"), 45.0);
	Rotation->SetNumberField(TEXT("roll"), 0.0);
	Args->SetObjectField(TEXT("rotation"), Rotation);
	TestTrue(TEXT("SetViewportCamera success"), MCPTestUtils::IsSuccess(SetCamTool.Execute(Args)));

	// Step 3: GetViewportCamera (verify new position)
	Mock.GetViewportCameraResult.Location = FVector(1000.0, 2000.0, 500.0);
	Mock.GetViewportCameraResult.Rotation = FRotator(-30.0, 45.0, 0.0);

	FGetViewportCameraImplTool GetCamTool2(Mock);
	Args = MakeShared<FJsonObject>();
	Result = GetCamTool2.Execute(Args);
	TestTrue(TEXT("GetViewportCamera verify success"), MCPTestUtils::IsSuccess(Result));

	// Step 4: SetCameraFov
	Mock.SetCameraFOVResult.bSuccess = true;
	Mock.SetCameraFOVResult.Message = TEXT("FOV set to 60.0");

	FSetCameraFovImplTool FovTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("fov"), 60.0);
	TestTrue(TEXT("SetCameraFov success"), MCPTestUtils::IsSuccess(FovTool.Execute(Args)));

	// Step 5: SetGameView (on)
	Mock.SetGameViewResult.bSuccess = true;
	Mock.SetGameViewResult.Message = TEXT("Game view enabled");

	FSetGameViewImplTool GameViewOnTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetBoolField(TEXT("enabled"), true);
	TestTrue(TEXT("SetGameView(on) success"), MCPTestUtils::IsSuccess(GameViewOnTool.Execute(Args)));

	// Step 6: SetGameView (off)
	Mock.SetGameViewResult.bSuccess = true;
	Mock.SetGameViewResult.Message = TEXT("Game view disabled");

	FSetGameViewImplTool GameViewOffTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetBoolField(TEXT("enabled"), false);
	TestTrue(TEXT("SetGameView(off) success"), MCPTestUtils::IsSuccess(GameViewOffTool.Execute(Args)));

	// Step 7: SetViewMode (Wireframe)
	Mock.SetViewModeResult.bSuccess = true;
	Mock.SetViewModeResult.Message = TEXT("View mode set to Wireframe");

	FSetViewModeImplTool ViewModeTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("mode"), TEXT("Wireframe"));
	TestTrue(TEXT("SetViewMode success"), MCPTestUtils::IsSuccess(ViewModeTool.Execute(Args)));

	// Step 8: SetBookmark (0)
	Mock.SetBookmarkResult.bSuccess = true;
	Mock.SetBookmarkResult.Message = TEXT("Bookmark 0 set");

	FSetBookmarkImplTool SetBmTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("index"), 0);
	TestTrue(TEXT("SetBookmark success"), MCPTestUtils::IsSuccess(SetBmTool.Execute(Args)));

	// Step 9: JumpToBookmark (0)
	Mock.JumpToBookmarkResult.bSuccess = true;
	Mock.JumpToBookmarkResult.Message = TEXT("Jumped to bookmark 0");

	FJumpToBookmarkImplTool JumpBmTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetNumberField(TEXT("index"), 0);
	TestTrue(TEXT("JumpToBookmark success"), MCPTestUtils::IsSuccess(JumpBmTool.Execute(Args)));

	// Verify all calls were made
	TestEqual(TEXT("GetViewportCamera called"), Mock.Recorder.GetCallCount(TEXT("GetViewportCamera")), 2);
	TestEqual(TEXT("SetViewportCamera called"), Mock.Recorder.GetCallCount(TEXT("SetViewportCamera")), 1);
	TestEqual(TEXT("SetCameraFOV called"), Mock.Recorder.GetCallCount(TEXT("SetCameraFOV")), 1);
	TestEqual(TEXT("SetGameView called"), Mock.Recorder.GetCallCount(TEXT("SetGameView")), 2);
	TestEqual(TEXT("SetViewMode called"), Mock.Recorder.GetCallCount(TEXT("SetViewMode")), 1);
	TestEqual(TEXT("SetBookmark called"), Mock.Recorder.GetCallCount(TEXT("SetBookmark")), 1);
	TestEqual(TEXT("JumpToBookmark called"), Mock.Recorder.GetCallCount(TEXT("JumpToBookmark")), 1);
	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 9);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
