// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockConsoleModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/GetCvarImplTool.h"
#include "Tools/Impl/SetCvarImplTool.h"
#include "Tools/Impl/ExecuteConsoleCommandImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FConsoleIntegrationTest,
	"MCPServer.Integration.Console.FullLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FConsoleIntegrationTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;

	// Step 1: Get CVar
	Mock.GetCVarResult.bSuccess = true;
	Mock.GetCVarResult.Name = TEXT("r.ScreenPercentage");
	Mock.GetCVarResult.Value = TEXT("100");
	FGetCvarImplTool GetTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("r.ScreenPercentage"));
	auto Result = GetTool.Execute(Args);
	TestTrue(TEXT("GetCVar success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("100")));

	// Step 2: Set CVar
	Mock.SetCVarResult.bSuccess = true;
	Mock.SetCVarResult.Name = TEXT("r.ScreenPercentage");
	Mock.SetCVarResult.Value = TEXT("75");
	FSetCvarImplTool SetTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("r.ScreenPercentage"));
	Args->SetStringField(TEXT("value"), TEXT("75"));
	TestTrue(TEXT("SetCVar success"), MCPTestUtils::IsSuccess(SetTool.Execute(Args)));

	// Step 3: Get CVar (verify new value)
	Mock.GetCVarResult.Value = TEXT("75");
	FGetCvarImplTool GetTool2(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("r.ScreenPercentage"));
	Result = GetTool2.Execute(Args);
	TestTrue(TEXT("GetCVar2 success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains new value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("75")));

	// Step 4: Execute console command
	Mock.ExecuteConsoleCommandResult.bSuccess = true;
	Mock.ExecuteConsoleCommandResult.Output = TEXT("Command executed");
	FExecuteConsoleCommandImplTool ExecTool(Mock);
	Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("command"), TEXT("stat fps"));
	TestTrue(TEXT("ExecuteCommand success"), MCPTestUtils::IsSuccess(ExecTool.Execute(Args)));

	TestEqual(TEXT("Total calls"), Mock.Recorder.GetTotalCallCount(), 4);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
