// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tests/Mocks/MockSCCModule.h"
#include "Tests/Integration/IntegrationTestUtils.h"
#include "Tools/Impl/SccStatusImplTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSourceControlIntegrationTest,
	"MCPServer.Integration.SourceControl.StatusCheck",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSourceControlIntegrationTest::RunTest(const FString& Parameters)
{
	FMockSCCModule Mock;

	// Step 1: Check status (may error gracefully if no provider)
	Mock.StatusResult.bSuccess = true;
	FSCCStatusResult::FFileStatus FileStatus;
	FileStatus.FilePath = TEXT("/Game/TestFile.uasset");
	FileStatus.bIsSourceControlled = true;
	FileStatus.bIsCurrent = true;
	Mock.StatusResult.FileStatuses.Add(FileStatus);

	FSccStatusImplTool StatusTool(Mock);
	auto Args = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> FilesArray;
	FilesArray.Add(MakeShared<FJsonValueString>(TEXT("/Game/TestFile.uasset")));
	Args->SetArrayField(TEXT("files"), FilesArray);
	auto Result = StatusTool.Execute(Args);
	TestTrue(TEXT("SCC Status success"), MCPTestUtils::IsSuccess(Result));

	TestEqual(TEXT("Status called"), Mock.Recorder.GetCallCount(TEXT("Status")), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
