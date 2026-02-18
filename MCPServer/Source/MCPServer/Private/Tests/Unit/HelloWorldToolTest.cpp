// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/Impl/HelloWorldImplTool.h"
#include "Tests/Integration/IntegrationTestUtils.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelloWorldMetadataTest,
	"MCPServer.Unit.HelloWorld.Metadata",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHelloWorldMetadataTest::RunTest(const FString& Parameters)
{
	FHelloWorldImplTool Tool;
	TestEqual(TEXT("Name"), Tool.GetName(), TEXT("hello_world"));
	TestTrue(TEXT("Description not empty"), !Tool.GetDescription().IsEmpty());
	TestTrue(TEXT("Schema valid"), Tool.GetInputSchema().IsValid());
	TestTrue(TEXT("Schema has type"), Tool.GetInputSchema()->HasField(TEXT("type")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelloWorldSuccessTest,
	"MCPServer.Unit.HelloWorld.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHelloWorldSuccessTest::RunTest(const FString& Parameters)
{
	FHelloWorldImplTool Tool;
	auto Args = MakeShared<FJsonObject>();
	Args->SetStringField(TEXT("name"), TEXT("TestUser"));
	auto Result = Tool.Execute(Args);

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains greeting"), MCPTestUtils::GetResultText(Result).Contains(TEXT("TestUser")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelloWorldDefaultNameTest,
	"MCPServer.Unit.HelloWorld.DefaultName",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHelloWorldDefaultNameTest::RunTest(const FString& Parameters)
{
	FHelloWorldImplTool Tool;
	auto Result = Tool.Execute(MakeShared<FJsonObject>());

	TestTrue(TEXT("Success"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains World"), MCPTestUtils::GetResultText(Result).Contains(TEXT("World")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelloWorldNullArgsTest,
	"MCPServer.Unit.HelloWorld.NullArgs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHelloWorldNullArgsTest::RunTest(const FString& Parameters)
{
	FHelloWorldImplTool Tool;
	auto Result = Tool.Execute(nullptr);

	TestTrue(TEXT("Success even with null args"), MCPTestUtils::IsSuccess(Result));
	TestTrue(TEXT("Contains World default"), MCPTestUtils::GetResultText(Result).Contains(TEXT("World")));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
