// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockProjectSettingsModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/GetProjectInfoImplTool.h"
#include "Tools/Impl/GetProjectPathsImplTool.h"
#include "Tools/Impl/SetConfigValueImplTool.h"
#include "Tools/Impl/GetConfigValueImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FProjectSettingsIntegrationTest,
	"MCPServer.Integration.ProjectSettings.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FProjectSettingsIntegrationTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;

	// Step 1: Get project info
	Mock.GetProjectInfoResult.bSuccess = true;
	Mock.GetProjectInfoResult.ProjectName = TEXT("TestProject");
	Mock.GetProjectInfoResult.EngineVersion = TEXT("5.7");
	FGetProjectInfoImplTool InfoTool(Mock);
	auto Result = InfoTool.Execute(MakeShared<FJsonObject>());
	TestTrue(TEXT("GetProjectInfo success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains project name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestProject")));

	// Step 2: Get project paths
	Mock.GetProjectPathsResult.bSuccess = true;
	Mock.GetProjectPathsResult.ProjectDir = TEXT("/Game");
	Mock.GetProjectPathsResult.ContentDir = TEXT("/Game/Content");
	FGetProjectPathsImplTool PathsTool(Mock);
	TestTrue(TEXT("GetProjectPaths success"), MCPTestUtils::IsSuccess(PathsTool.Execute(MakeShared<FJsonObject>())));

	// Step 3: Set config value
	Mock.SetConfigValueResult.bSuccess = true;
	Mock.SetConfigValueResult.Section = TEXT("/Script/Engine.Engine");
	Mock.SetConfigValueResult.Key = TEXT("TestKey");
	Mock.SetConfigValueResult.Value = TEXT("TestValue");
	FSetConfigValueImplTool SetTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("config"), TEXT("Engine"));
	Args->SetStringField(TEXT("section"), TEXT("/Script/Engine.Engine"));
	Args->SetStringField(TEXT("key"), TEXT("TestKey"));
	Args->SetStringField(TEXT("value"), TEXT("TestValue"));
	TestTrue(TEXT("SetConfigValue success"), MCPTestUtils::IsSuccess(SetTool.Execute(Args)));

	// Step 4: Get config value (verify)
	Mock.GetConfigValueResult.bSuccess = true;
	Mock.GetConfigValueResult.Value = TEXT("TestValue");
	FGetConfigValueImplTool GetTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("config"), TEXT("Engine"));
	Args->SetStringField(TEXT("section"), TEXT("/Script/Engine.Engine"));
	Args->SetStringField(TEXT("key"), TEXT("TestKey"));
	Result = GetTool.Execute(Args);
	TestTrue(TEXT("GetConfigValue success"), MCPTestUtils::IsSuccess(Result));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 4);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
