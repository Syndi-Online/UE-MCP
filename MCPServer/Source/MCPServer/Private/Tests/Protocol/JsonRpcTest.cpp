// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Protocol/MCPJsonRpc.h"
#include "Protocol/MCPSessionManager.h"
#include "Tools/MCPToolRegistry.h"
#include "Tools/Interfaces/IMCPTool.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#if WITH_DEV_AUTOMATION_TESTS

/** Simple tool for JsonRpc tests */
class FJsonRpcTestTool : public IMCPTool
{
public:
	virtual FString GetName() const override { return TEXT("test_echo"); }
	virtual FString GetDescription() const override { return TEXT("Echo tool for tests"); }
	virtual TSharedPtr<FJsonObject> GetInputSchema() const override
	{
		auto Schema = MakeShared<FJsonObject>();
		Schema->SetStringField(TEXT("type"), TEXT("object"));
		return Schema;
	}
	virtual TSharedPtr<FJsonObject> Execute(const TSharedPtr<FJsonObject>& Arguments) override
	{
		auto Result = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> ContentArray;
		auto TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("echo_ok"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), false);
		return Result;
	}
};

namespace JsonRpcTestHelpers
{
	struct FTestContext
	{
		FMCPToolRegistry Registry;
		FMCPSessionManager SessionManager;
		TUniquePtr<FMCPJsonRpc> JsonRpc;

		FTestContext()
		{
			Registry.RegisterTool(MakeShared<FJsonRpcTestTool>());
			JsonRpc = MakeUnique<FMCPJsonRpc>(Registry, SessionManager);
		}

		void Process(const FString& Body, FString& OutResponse, FString& OutNewSessionId, int32& OutHttpCode, const FString& SessionId = TEXT(""))
		{
			JsonRpc->ProcessMessage(Body, SessionId, OutResponse, OutNewSessionId, OutHttpCode);
		}
	};

	TSharedPtr<FJsonObject> ParseResponse(const FString& Response)
	{
		TSharedPtr<FJsonValue> Parsed;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response);
		FJsonSerializer::Deserialize(Reader, Parsed);
		if (Parsed.IsValid() && Parsed->Type == EJson::Object)
		{
			return Parsed->AsObject();
		}
		return nullptr;
	}
}

// ---- Initialize ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcInitializeTest,
	"MCPServer.Protocol.JsonRpc.Initialize",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcInitializeTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\",\"params\":{}}"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 200"), HttpCode, 200);
	TestTrue(TEXT("New session ID created"), !NewSessionId.IsEmpty());

	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Response is valid JSON"), Obj.IsValid());

	const TSharedPtr<FJsonObject>* ResultObj;
	TestTrue(TEXT("Has result"), Obj->TryGetObjectField(TEXT("result"), ResultObj));

	FString ProtocolVersion;
	(*ResultObj)->TryGetStringField(TEXT("protocolVersion"), ProtocolVersion);
	TestEqual(TEXT("Protocol version"), ProtocolVersion, TEXT("2025-03-26"));
	return true;
}

// ---- Ping ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcPingTest,
	"MCPServer.Protocol.JsonRpc.Ping",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcPingTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"ping\"}"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 200"), HttpCode, 200);
	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has result"), Obj.IsValid() && Obj->HasField(TEXT("result")));
	return true;
}

// ---- ToolsList ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcToolsListTest,
	"MCPServer.Protocol.JsonRpc.ToolsList",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcToolsListTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"tools/list\",\"params\":{}}"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 200"), HttpCode, 200);
	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Valid response"), Obj.IsValid());

	const TSharedPtr<FJsonObject>* ResultObj;
	TestTrue(TEXT("Has result"), Obj->TryGetObjectField(TEXT("result"), ResultObj));

	const TArray<TSharedPtr<FJsonValue>>* ToolsArray;
	TestTrue(TEXT("Has tools array"), (*ResultObj)->TryGetArrayField(TEXT("tools"), ToolsArray));
	TestEqual(TEXT("One tool registered"), ToolsArray->Num(), 1);
	return true;
}

// ---- ToolsCall Success ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcToolsCallSuccessTest,
	"MCPServer.Protocol.JsonRpc.ToolsCall.Success",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcToolsCallSuccessTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":4,\"method\":\"tools/call\",\"params\":{\"name\":\"test_echo\",\"arguments\":{}}}"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 200"), HttpCode, 200);
	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	const TSharedPtr<FJsonObject>* ResultObj;
	TestTrue(TEXT("Has result"), Obj->TryGetObjectField(TEXT("result"), ResultObj));

	bool bIsError = true;
	(*ResultObj)->TryGetBoolField(TEXT("isError"), bIsError);
	TestFalse(TEXT("Not an error"), bIsError);
	return true;
}

// ---- ToolsCall Unknown ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcToolsCallUnknownTest,
	"MCPServer.Protocol.JsonRpc.ToolsCall.Unknown",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcToolsCallUnknownTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":5,\"method\":\"tools/call\",\"params\":{\"name\":\"nonexistent\",\"arguments\":{}}}"), Response, NewSessionId, HttpCode);

	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has error"), Obj->HasField(TEXT("error")));
	return true;
}

// ---- ToolsCall No Params ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcToolsCallNoParamsTest,
	"MCPServer.Protocol.JsonRpc.ToolsCall.NoParams",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcToolsCallNoParamsTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":6,\"method\":\"tools/call\"}"), Response, NewSessionId, HttpCode);

	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has error"), Obj->HasField(TEXT("error")));
	return true;
}

// ---- ToolsCall No Name ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcToolsCallNoNameTest,
	"MCPServer.Protocol.JsonRpc.ToolsCall.NoName",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcToolsCallNoNameTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"tools/call\",\"params\":{}}"), Response, NewSessionId, HttpCode);

	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has error for missing name"), Obj->HasField(TEXT("error")));
	return true;
}

// ---- Parse Error ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcParseErrorTest,
	"MCPServer.Protocol.JsonRpc.ParseError",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcParseErrorTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{invalid json!!!"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 400"), HttpCode, 400);
	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has error"), Obj.IsValid() && Obj->HasField(TEXT("error")));
	return true;
}

// ---- Invalid Request ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcInvalidRequestTest,
	"MCPServer.Protocol.JsonRpc.InvalidRequest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcInvalidRequestTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	// Send a JSON string value instead of object/array
	Ctx.Process(TEXT("\"just a string\""), Response, NewSessionId, HttpCode);
	TestEqual(TEXT("HTTP 400"), HttpCode, 400);
	return true;
}

// ---- Missing Version ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcMissingVersionTest,
	"MCPServer.Protocol.JsonRpc.MissingVersion",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcMissingVersionTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"id\":1,\"method\":\"ping\"}"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 400"), HttpCode, 400);
	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has error for missing jsonrpc version"), Obj.IsValid() && Obj->HasField(TEXT("error")));
	return true;
}

// ---- Method Not Found ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcMethodNotFoundTest,
	"MCPServer.Protocol.JsonRpc.MethodNotFound",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcMethodNotFoundTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"id\":10,\"method\":\"unknown/method\"}"), Response, NewSessionId, HttpCode);

	auto Obj = JsonRpcTestHelpers::ParseResponse(Response);
	TestTrue(TEXT("Has error"), Obj.IsValid() && Obj->HasField(TEXT("error")));
	return true;
}

// ---- Notification returns 202 ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcNotificationTest,
	"MCPServer.Protocol.JsonRpc.Notification",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcNotificationTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("{\"jsonrpc\":\"2.0\",\"method\":\"notifications/initialized\"}"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 202 for notification"), HttpCode, 202);
	return true;
}

// ---- Batch (2 requests) ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcBatchRequestsTest,
	"MCPServer.Protocol.JsonRpc.Batch.Requests",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcBatchRequestsTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("[{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"ping\"},{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"ping\"}]"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 200"), HttpCode, 200);
	// Parse as array
	TSharedPtr<FJsonValue> Parsed;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response);
	FJsonSerializer::Deserialize(Reader, Parsed);
	TestTrue(TEXT("Response is array"), Parsed.IsValid() && Parsed->Type == EJson::Array);
	TestEqual(TEXT("Two responses"), Parsed->AsArray().Num(), 2);
	return true;
}

// ---- Batch (all notifications) ----
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FJsonRpcBatchNotificationsTest,
	"MCPServer.Protocol.JsonRpc.Batch.AllNotifications",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FJsonRpcBatchNotificationsTest::RunTest(const FString& Parameters)
{
	JsonRpcTestHelpers::FTestContext Ctx;
	FString Response, NewSessionId;
	int32 HttpCode;
	Ctx.Process(TEXT("[{\"jsonrpc\":\"2.0\",\"method\":\"notifications/initialized\"},{\"jsonrpc\":\"2.0\",\"method\":\"notifications/cancelled\"}]"), Response, NewSessionId, HttpCode);

	TestEqual(TEXT("HTTP 202 for all-notification batch"), HttpCode, 202);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
