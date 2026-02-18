// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Tools/MCPToolRegistry.h"
#include "Tools/Interfaces/IMCPTool.h"
#include "Dom/JsonValue.h"

#if WITH_DEV_AUTOMATION_TESTS

/** Simple test tool for registry tests */
class FTestTool : public IMCPTool
{
public:
	FString Name;
	bool bShouldSucceed = true;

	FTestTool(const FString& InName) : Name(InName) {}

	virtual FString GetName() const override { return Name; }
	virtual FString GetDescription() const override { return FString::Printf(TEXT("Test tool: %s"), *Name); }
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override
	{
		TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		return Schema;
	}
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override
	{
		TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> ContentArray;
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), FString::Printf(TEXT("Executed: %s"), *Name));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), !bShouldSucceed);
		return Result;
	}
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FToolRegistryRegisterHasTest,
	"MCPServer.Protocol.ToolRegistry.RegisterAndHas",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FToolRegistryRegisterHasTest::RunTest(const FString& Parameters)
{
	FMCPToolRegistry Registry;
	auto Tool = MakeShared<FTestTool>(TEXT("test_tool"));
	Registry.RegisterTool(Tool);
	TestTrue(TEXT("Registry should have the registered tool"), Registry.HasTool(TEXT("test_tool")));
	TestFalse(TEXT("Registry should not have unregistered tool"), Registry.HasTool(TEXT("nonexistent")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FToolRegistryRegisterNullTest,
	"MCPServer.Protocol.ToolRegistry.RegisterNull",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FToolRegistryRegisterNullTest::RunTest(const FString& Parameters)
{
	FMCPToolRegistry Registry;
	Registry.RegisterTool(nullptr);
	TestEqual(TEXT("GetToolsList should be empty after null registration"), Registry.GetToolsList().Num(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FToolRegistryCallToolTest,
	"MCPServer.Protocol.ToolRegistry.CallTool",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FToolRegistryCallToolTest::RunTest(const FString& Parameters)
{
	FMCPToolRegistry Registry;
	Registry.RegisterTool(MakeShared<FTestTool>(TEXT("my_tool")));
	auto Args = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> Result = Registry.CallTool(TEXT("my_tool"), Args);
	TestTrue(TEXT("CallTool should return a valid result"), Result.IsValid());

	bool bIsError = true;
	Result->TryGetBoolField(TEXT("isError"), bIsError);
	TestFalse(TEXT("Result should not be an error"), bIsError);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FToolRegistryCallUnknownTest,
	"MCPServer.Protocol.ToolRegistry.CallUnknown",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FToolRegistryCallUnknownTest::RunTest(const FString& Parameters)
{
	FMCPToolRegistry Registry;
	auto Args = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> Result = Registry.CallTool(TEXT("nonexistent"), Args);
	TestFalse(TEXT("Calling unknown tool should return nullptr"), Result.IsValid());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FToolRegistryGetToolsListTest,
	"MCPServer.Protocol.ToolRegistry.GetToolsList",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FToolRegistryGetToolsListTest::RunTest(const FString& Parameters)
{
	FMCPToolRegistry Registry;
	Registry.RegisterTool(MakeShared<FTestTool>(TEXT("tool_a")));
	Registry.RegisterTool(MakeShared<FTestTool>(TEXT("tool_b")));
	Registry.RegisterTool(MakeShared<FTestTool>(TEXT("tool_c")));

	TArray<TSharedPtr<FJsonValue>> ToolsList = Registry.GetToolsList();
	TestEqual(TEXT("Should have 3 tools"), ToolsList.Num(), 3);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FToolRegistryDuplicateTest,
	"MCPServer.Protocol.ToolRegistry.Duplicate",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FToolRegistryDuplicateTest::RunTest(const FString& Parameters)
{
	FMCPToolRegistry Registry;
	Registry.RegisterTool(MakeShared<FTestTool>(TEXT("dup_tool")));
	Registry.RegisterTool(MakeShared<FTestTool>(TEXT("dup_tool")));
	// Second registration should overwrite the first
	TestTrue(TEXT("Tool should still exist"), Registry.HasTool(TEXT("dup_tool")));
	TArray<TSharedPtr<FJsonValue>> ToolsList = Registry.GetToolsList();
	TestEqual(TEXT("Should still have 1 tool (overwritten)"), ToolsList.Num(), 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
