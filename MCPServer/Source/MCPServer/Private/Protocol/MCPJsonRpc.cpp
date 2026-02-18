// Copyright Epic Games, Inc. All Rights Reserved.

#include "Protocol/MCPJsonRpc.h"
#include "Protocol/MCPSessionManager.h"
#include "Tools/MCPToolRegistry.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

#define MCP_PROTOCOL_VERSION TEXT("2025-03-26")
#define MCP_SERVER_NAME TEXT("UnrealEngine-MCPServer")
#define MCP_SERVER_VERSION TEXT("1.0.0")

FMCPJsonRpc::FMCPJsonRpc(FMCPToolRegistry& InToolRegistry, FMCPSessionManager& InSessionManager)
	: ToolRegistry(InToolRegistry)
	, SessionManager(InSessionManager)
{
}

void FMCPJsonRpc::ProcessMessage(
	const FString& RequestBody,
	const FString& SessionId,
	FString& OutResponse,
	FString& OutNewSessionId,
	int32& OutHttpCode)
{
	OutResponse.Empty();
	OutNewSessionId.Empty();
	OutHttpCode = 200;

	TSharedPtr<FJsonValue> ParsedValue;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(RequestBody);
	if (!FJsonSerializer::Deserialize(Reader, ParsedValue) || !ParsedValue.IsValid())
	{
		TSharedPtr<FJsonObject> ErrorResponse = MakeError(nullptr, -32700, TEXT("Parse error"));
		OutResponse = SerializeJson(ErrorResponse);
		OutHttpCode = 400;
		return;
	}

	if (ParsedValue->Type == EJson::Array)
	{
		const TArray<TSharedPtr<FJsonValue>>& BatchArray = ParsedValue->AsArray();
		TArray<TSharedPtr<FJsonValue>> Responses;
		bool bHasRequests = false;

		for (const TSharedPtr<FJsonValue>& Item : BatchArray)
		{
			if (Item->Type != EJson::Object)
			{
				continue;
			}

			TSharedPtr<FJsonObject> ResponseObj;
			HandleSingleMessage(Item->AsObject(), SessionId, ResponseObj, OutNewSessionId, OutHttpCode);

			if (ResponseObj.IsValid())
			{
				Responses.Add(MakeShared<FJsonValueObject>(ResponseObj));
				bHasRequests = true;
			}
		}

		if (bHasRequests)
		{
			FString OutputString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(Responses, Writer);
			OutResponse = OutputString;
			OutHttpCode = 200;
		}
		else
		{
			OutHttpCode = 202;
		}
	}
	else if (ParsedValue->Type == EJson::Object)
	{
		TSharedPtr<FJsonObject> MessageObj = ParsedValue->AsObject();
		TSharedPtr<FJsonObject> ResponseObj;

		HandleSingleMessage(MessageObj, SessionId, ResponseObj, OutNewSessionId, OutHttpCode);

		if (ResponseObj.IsValid())
		{
			OutResponse = SerializeJson(ResponseObj);
		}
	}
	else
	{
		TSharedPtr<FJsonObject> ErrorResponse = MakeError(nullptr, -32600, TEXT("Invalid Request"));
		OutResponse = SerializeJson(ErrorResponse);
		OutHttpCode = 400;
	}
}

void FMCPJsonRpc::HandleSingleMessage(
	const TSharedPtr<FJsonObject>& Message,
	const FString& SessionId,
	TSharedPtr<FJsonObject>& OutResponse,
	FString& OutNewSessionId,
	int32& OutHttpCode)
{
	FString JsonRpcVersion;
	if (!Message->TryGetStringField(TEXT("jsonrpc"), JsonRpcVersion) || JsonRpcVersion != TEXT("2.0"))
	{
		OutResponse = MakeError(nullptr, -32600, TEXT("Invalid Request: missing or invalid jsonrpc version"));
		OutHttpCode = 400;
		return;
	}

	FString Method;
	if (!Message->TryGetStringField(TEXT("method"), Method))
	{
		OutHttpCode = 202;
		return;
	}

	TSharedPtr<FJsonValue> IdValue = Message->TryGetField(TEXT("id"));
	bool bIsNotification = !IdValue.IsValid() || IdValue->IsNull();

	TSharedPtr<FJsonObject> Params;
	const TSharedPtr<FJsonObject>* ParamsPtr;
	if (Message->TryGetObjectField(TEXT("params"), ParamsPtr))
	{
		Params = *ParamsPtr;
	}

	if (Method == TEXT("initialize"))
	{
		OutResponse = HandleInitialize(IdValue, Params, OutNewSessionId);
		OutHttpCode = 200;
	}
	else if (Method == TEXT("notifications/initialized"))
	{
		OutHttpCode = 202;
	}
	else if (Method == TEXT("ping"))
	{
		OutResponse = HandlePing(IdValue);
		OutHttpCode = 200;
	}
	else if (Method == TEXT("tools/list"))
	{
		OutResponse = HandleToolsList(IdValue, Params);
		OutHttpCode = 200;
	}
	else if (Method == TEXT("tools/call"))
	{
		OutResponse = HandleToolsCall(IdValue, Params);
		OutHttpCode = 200;
	}
	else if (Method.StartsWith(TEXT("notifications/")))
	{
		OutHttpCode = 202;
	}
	else
	{
		if (!bIsNotification)
		{
			OutResponse = MakeError(IdValue, -32601, FString::Printf(TEXT("Method not found: %s"), *Method));
			OutHttpCode = 200;
		}
		else
		{
			OutHttpCode = 202;
		}
	}
}

TSharedPtr<FJsonObject> FMCPJsonRpc::HandleInitialize(
	const TSharedPtr<FJsonValue>& Id,
	const TSharedPtr<FJsonObject>& Params,
	FString& OutNewSessionId)
{
	OutNewSessionId = SessionManager.CreateSession();

	TSharedPtr<FJsonObject> ToolsCap = MakeShared<FJsonObject>();
	ToolsCap->SetBoolField(TEXT("listChanged"), false);

	TSharedPtr<FJsonObject> Capabilities = MakeShared<FJsonObject>();
	Capabilities->SetObjectField(TEXT("tools"), ToolsCap);

	TSharedPtr<FJsonObject> ServerInfo = MakeShared<FJsonObject>();
	ServerInfo->SetStringField(TEXT("name"), MCP_SERVER_NAME);
	ServerInfo->SetStringField(TEXT("version"), MCP_SERVER_VERSION);

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetStringField(TEXT("protocolVersion"), MCP_PROTOCOL_VERSION);
	Result->SetObjectField(TEXT("capabilities"), Capabilities);
	Result->SetObjectField(TEXT("serverInfo"), ServerInfo);

	return MakeResponse(Id, Result);
}

TSharedPtr<FJsonObject> FMCPJsonRpc::HandleToolsList(
	const TSharedPtr<FJsonValue>& Id,
	const TSharedPtr<FJsonObject>& Params)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	Result->SetArrayField(TEXT("tools"), ToolRegistry.GetToolsList());
	return MakeResponse(Id, Result);
}

TSharedPtr<FJsonObject> FMCPJsonRpc::HandleToolsCall(
	const TSharedPtr<FJsonValue>& Id,
	const TSharedPtr<FJsonObject>& Params)
{
	if (!Params.IsValid())
	{
		return MakeError(Id, -32602, TEXT("Invalid params: params required"));
	}

	FString ToolName;
	if (!Params->TryGetStringField(TEXT("name"), ToolName))
	{
		return MakeError(Id, -32602, TEXT("Invalid params: missing tool name"));
	}

	if (!ToolRegistry.HasTool(ToolName))
	{
		return MakeError(Id, -32602, FString::Printf(TEXT("Unknown tool: %s"), *ToolName));
	}

	TSharedPtr<FJsonObject> Arguments;
	const TSharedPtr<FJsonObject>* ArgumentsPtr;
	if (Params->TryGetObjectField(TEXT("arguments"), ArgumentsPtr))
	{
		Arguments = *ArgumentsPtr;
	}
	else
	{
		Arguments = MakeShared<FJsonObject>();
	}

	TSharedPtr<FJsonObject> ToolResult = ToolRegistry.CallTool(ToolName, Arguments);
	if (!ToolResult.IsValid())
	{
		TSharedPtr<FJsonObject> ErrorResult = MakeShared<FJsonObject>();

		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), FString::Printf(TEXT("Tool '%s' execution failed"), *ToolName));

		TArray<TSharedPtr<FJsonValue>> ContentArray;
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		ErrorResult->SetArrayField(TEXT("content"), ContentArray);
		ErrorResult->SetBoolField(TEXT("isError"), true);

		return MakeResponse(Id, ErrorResult);
	}

	return MakeResponse(Id, ToolResult);
}

TSharedPtr<FJsonObject> FMCPJsonRpc::HandlePing(const TSharedPtr<FJsonValue>& Id)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	return MakeResponse(Id, Result);
}

TSharedPtr<FJsonObject> FMCPJsonRpc::MakeResponse(const TSharedPtr<FJsonValue>& Id, const TSharedPtr<FJsonObject>& Result)
{
	TSharedPtr<FJsonObject> Response = MakeShared<FJsonObject>();
	Response->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));

	if (Id.IsValid() && !Id->IsNull())
	{
		Response->SetField(TEXT("id"), Id);
	}
	else
	{
		Response->SetField(TEXT("id"), MakeShared<FJsonValueNull>());
	}

	Response->SetObjectField(TEXT("result"), Result);
	return Response;
}

TSharedPtr<FJsonObject> FMCPJsonRpc::MakeError(const TSharedPtr<FJsonValue>& Id, int32 Code, const FString& Message)
{
	TSharedPtr<FJsonObject> ErrorObj = MakeShared<FJsonObject>();
	ErrorObj->SetNumberField(TEXT("code"), Code);
	ErrorObj->SetStringField(TEXT("message"), Message);

	TSharedPtr<FJsonObject> Response = MakeShared<FJsonObject>();
	Response->SetStringField(TEXT("jsonrpc"), TEXT("2.0"));

	if (Id.IsValid() && !Id->IsNull())
	{
		Response->SetField(TEXT("id"), Id);
	}
	else
	{
		Response->SetField(TEXT("id"), MakeShared<FJsonValueNull>());
	}

	Response->SetObjectField(TEXT("error"), ErrorObj);
	return Response;
}

FString FMCPJsonRpc::SerializeJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return OutputString;
}
