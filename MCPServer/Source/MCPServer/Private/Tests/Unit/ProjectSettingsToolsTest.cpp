// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/GetProjectInfoImplTool.h"
#include "Tools/Impl/GetConfigValueImplTool.h"
#include "Tools/Impl/SetConfigValueImplTool.h"
#include "Tools/Impl/GetProjectPathsImplTool.h"
#include "Tests/Mocks/MockProjectSettingsModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

// ============================================================================
// GetProjectInfo
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectInfoMetadataTest,
	"MCPServer.Unit.ProjectSettings.GetProjectInfo.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectInfoMetadataTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	FGetProjectInfoImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_project_info"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectInfoSuccessTest,
	"MCPServer.Unit.ProjectSettings.GetProjectInfo.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectInfoSuccessTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetProjectInfoResult.bSuccess = true;
	Mock.GetProjectInfoResult.ProjectName = TEXT("TestProject");
	Mock.GetProjectInfoResult.EngineVersion = TEXT("5.7");

	FGetProjectInfoImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains project name"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestProject")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetProjectInfo")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectInfoEmptyArgsTest,
	"MCPServer.Unit.ProjectSettings.GetProjectInfo.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectInfoEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetProjectInfoResult.bSuccess = true;

	FGetProjectInfoImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetProjectInfo")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectInfoModuleFailureTest,
	"MCPServer.Unit.ProjectSettings.GetProjectInfo.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectInfoModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetProjectInfoResult.bSuccess = false;
	Mock.GetProjectInfoResult.ErrorMessage = TEXT("Failed to read project info");

	FGetProjectInfoImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetProjectInfo")), 1);
	return true;
}

// ============================================================================
// GetConfigValue
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetConfigValueMetadataTest,
	"MCPServer.Unit.ProjectSettings.GetConfigValue.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetConfigValueMetadataTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	FGetConfigValueImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_config_value"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetConfigValueSuccessTest,
	"MCPServer.Unit.ProjectSettings.GetConfigValue.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetConfigValueSuccessTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetConfigValueResult.bSuccess = true;
	Mock.GetConfigValueResult.Value = TEXT("TestVal");

	FGetConfigValueImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("config"), TEXT("Engine"));
	Args->SetStringField(TEXT("section"), TEXT("/Script/Engine.RendererSettings"));
	Args->SetStringField(TEXT("key"), TEXT("r.DefaultFeature.AutoExposure"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains value"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestVal")));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetConfigValue")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetConfigValueMissingArgsTest,
	"MCPServer.Unit.ProjectSettings.GetConfigValue.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetConfigValueMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	FGetConfigValueImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("GetConfigValue")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetConfigValueModuleFailureTest,
	"MCPServer.Unit.ProjectSettings.GetConfigValue.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetConfigValueModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetConfigValueResult.bSuccess = false;
	Mock.GetConfigValueResult.ErrorMessage = TEXT("Key not found");

	FGetConfigValueImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("config"), TEXT("Engine"));
	Args->SetStringField(TEXT("section"), TEXT("Missing"));
	Args->SetStringField(TEXT("key"), TEXT("Missing"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetConfigValue")), 1);
	return true;
}

// ============================================================================
// SetConfigValue
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetConfigValueMetadataTest,
	"MCPServer.Unit.ProjectSettings.SetConfigValue.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetConfigValueMetadataTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	FSetConfigValueImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("set_config_value"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetConfigValueSuccessTest,
	"MCPServer.Unit.ProjectSettings.SetConfigValue.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetConfigValueSuccessTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.SetConfigValueResult.bSuccess = true;

	FSetConfigValueImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("config"), TEXT("Engine"));
	Args->SetStringField(TEXT("section"), TEXT("/Script/Engine.RendererSettings"));
	Args->SetStringField(TEXT("key"), TEXT("r.DefaultFeature.AutoExposure"));
	Args->SetStringField(TEXT("value"), TEXT("True"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetConfigValue")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetConfigValueMissingArgsTest,
	"MCPServer.Unit.ProjectSettings.SetConfigValue.MissingArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetConfigValueMissingArgsTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	FSetConfigValueImplTool Tool(Mock);
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Error on missing args"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module not called"), Mock.Recorder.GetCallCount(TEXT("SetConfigValue")), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSetConfigValueModuleFailureTest,
	"MCPServer.Unit.ProjectSettings.SetConfigValue.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSetConfigValueModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.SetConfigValueResult.bSuccess = false;
	Mock.SetConfigValueResult.ErrorMessage = TEXT("Config file read-only");

	FSetConfigValueImplTool Tool(Mock);
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("config"), TEXT("Engine"));
	Args->SetStringField(TEXT("section"), TEXT("Section"));
	Args->SetStringField(TEXT("key"), TEXT("Key"));
	Args->SetStringField(TEXT("value"), TEXT("Val"));

	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("SetConfigValue")), 1);
	return true;
}

// ============================================================================
// GetProjectPaths
// ============================================================================

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectPathsMetadataTest,
	"MCPServer.Unit.ProjectSettings.GetProjectPaths.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectPathsMetadataTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	FGetProjectPathsImplTool Tool(Mock);
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("get_project_paths"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectPathsSuccessTest,
	"MCPServer.Unit.ProjectSettings.GetProjectPaths.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectPathsSuccessTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetProjectPathsResult.bSuccess = true;
	Mock.GetProjectPathsResult.ProjectDir = TEXT("/Game");
	Mock.GetProjectPathsResult.ContentDir = TEXT("/Content");

	FGetProjectPathsImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetProjectPaths")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectPathsEmptyArgsTest,
	"MCPServer.Unit.ProjectSettings.GetProjectPaths.EmptyArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectPathsEmptyArgsTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetProjectPathsResult.bSuccess = true;

	FGetProjectPathsImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Success with empty args"), MCPTestUtils::IsSuccess(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetProjectPaths")), 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGetProjectPathsModuleFailureTest,
	"MCPServer.Unit.ProjectSettings.GetProjectPaths.ModuleFailure",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FGetProjectPathsModuleFailureTest::RunTest(const FString& Parameters)
{
	FMockProjectSettingsModule Mock;
	Mock.GetProjectPathsResult.bSuccess = false;
	Mock.GetProjectPathsResult.ErrorMessage = TEXT("Failed to get paths");

	FGetProjectPathsImplTool Tool(Mock);
	auto Result = Tool.Execute(MCPTestUtils::EmptyArgs());

	TestTrue(TEXT("Error on module failure"), MCPTestUtils::IsError(Result));
	TestEqual(TEXT("Module called"), Mock.Recorder.GetCallCount(TEXT("GetProjectPaths")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
