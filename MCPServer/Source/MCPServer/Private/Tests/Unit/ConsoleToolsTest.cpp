// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/ExecuteConsoleCommandImplTool.h"
#include "Tools/Impl/GetCvarImplTool.h"
#include "Tools/Impl/SetCvarImplTool.h"
#include "Tests/Mocks/MockConsoleModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// ExecuteConsoleCommand
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExecuteConsoleCommandMetadataTest,
	"MCPServer.Unit.Console.ExecuteConsoleCommand.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExecuteConsoleCommandMetadataTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	FExecuteConsoleCommandImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("execute_console_command"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExecuteConsoleCommandSuccessTest,
	"MCPServer.Unit.Console.ExecuteConsoleCommand.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExecuteConsoleCommandSuccessTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	Mock.ExecuteConsoleCommandResult.bSuccess = true;
	Mock.ExecuteConsoleCommandResult.Output = TEXT("Done");

	FExecuteConsoleCommandImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("command"), TEXT("stat fps"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains output"), MCPTestUtils::GetResultText(Result).Contains(TEXT("Done")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ExecuteConsoleCommand")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExecuteConsoleCommandMissingArgsTest,
	"MCPServer.Unit.Console.ExecuteConsoleCommand.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExecuteConsoleCommandMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	FExecuteConsoleCommandImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("ExecuteConsoleCommand")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FExecuteConsoleCommandModuleFailureTest,
	"MCPServer.Unit.Console.ExecuteConsoleCommand.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FExecuteConsoleCommandModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	Mock.ExecuteConsoleCommandResult.bSuccess = false;
	Mock.ExecuteConsoleCommandResult.ErrorMessage = TEXT("Unknown command");

	FExecuteConsoleCommandImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("command"), TEXT("invalid_command"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("ExecuteConsoleCommand")), 1);
	return true;
}

// ============================================================================
// GetCVar
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetCVarMetadataTest,
	"MCPServer.Unit.Console.GetCVar.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetCVarMetadataTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	FGetCvarImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_cvar"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetCVarSuccessTest,
	"MCPServer.Unit.Console.GetCVar.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetCVarSuccessTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	Mock.GetCVarResult.bSuccess = true;
	Mock.GetCVarResult.Name = TEXT("r.ScreenPercentage");
	Mock.GetCVarResult.Value = TEXT("100");

	FGetCvarImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("r.ScreenPercentage"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("100")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetCVar")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetCVarMissingArgsTest,
	"MCPServer.Unit.Console.GetCVar.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetCVarMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	FGetCvarImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("GetCVar")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetCVarModuleFailureTest,
	"MCPServer.Unit.Console.GetCVar.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetCVarModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	Mock.GetCVarResult.bSuccess = false;
	Mock.GetCVarResult.ErrorMessage = TEXT("CVar not found");

	FGetCvarImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("invalid.cvar"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetCVar")), 1);
	return true;
}

// ============================================================================
// SetCVar
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCVarMetadataTest,
	"MCPServer.Unit.Console.SetCVar.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCVarMetadataTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	FSetCvarImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_cvar"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCVarSuccessTest,
	"MCPServer.Unit.Console.SetCVar.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCVarSuccessTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	Mock.SetCVarResult.bSuccess = true;
	Mock.SetCVarResult.Name = TEXT("r.ScreenPercentage");
	Mock.SetCVarResult.Value = TEXT("75");

	FSetCvarImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("r.ScreenPercentage"));
	Args->SetStringField(TEXT("value"), TEXT("75"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("75")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetCVar")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCVarMissingArgsTest,
	"MCPServer.Unit.Console.SetCVar.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCVarMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	FSetCvarImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("SetCVar")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetCVarModuleFailureTest,
	"MCPServer.Unit.Console.SetCVar.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetCVarModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockConsoleModule Mock;
	Mock.SetCVarResult.bSuccess = false;
	Mock.SetCVarResult.ErrorMessage = TEXT("CVar is read-only");

	FSetCvarImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("r.ReadOnly"));
	Args->SetStringField(TEXT("value"), TEXT("1"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetCVar")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
