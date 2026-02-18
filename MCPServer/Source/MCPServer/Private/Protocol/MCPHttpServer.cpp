// Copyright Epic Games, Inc. All Rights Reserved.

#include "Protocol/MCPHttpServer.h"
#include "Protocol/MCPJsonRpc.h"
#include "Protocol/MCPSessionManager.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "HttpPath.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMCPHttpServer, Log, All);
DEFINE_LOG_CATEGORY(LogMCPHttpServer);

FMCPHttpServer::FMCPHttpServer(FMCPJsonRpc& InJsonRpc, FMCPSessionManager& InSessionManager)
	: JsonRpc(InJsonRpc)
	, SessionManager(InSessionManager)
{
}

void FMCPHttpServer::Start(uint32 Port)
{
	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	HttpRouter = HttpServerModule.GetHttpRouter(Port);

	if (!HttpRouter.IsValid())
	{
		UE_LOG(LogMCPHttpServer, Error, TEXT("Failed to create HTTP router on port %d"), Port);
		return;
	}

	PostRouteHandle = HttpRouter->BindRoute(
		FHttpPath(TEXT("/mcp")),
		EHttpServerRequestVerbs::VERB_POST,
		FHttpRequestHandler::CreateRaw(this, &FMCPHttpServer::HandlePostRequest));

	DeleteRouteHandle = HttpRouter->BindRoute(
		FHttpPath(TEXT("/mcp")),
		EHttpServerRequestVerbs::VERB_DELETE,
		FHttpRequestHandler::CreateRaw(this, &FMCPHttpServer::HandleDeleteRequest));

	HttpServerModule.StartAllListeners();

	UE_LOG(LogMCPHttpServer, Log, TEXT("MCP HTTP Server started on port %d"), Port);
}

void FMCPHttpServer::Stop()
{
	if (HttpRouter.IsValid())
	{
		HttpRouter->UnbindRoute(PostRouteHandle);
		HttpRouter->UnbindRoute(DeleteRouteHandle);
	}

	FHttpServerModule::Get().StopAllListeners();

	UE_LOG(LogMCPHttpServer, Log, TEXT("MCP HTTP Server stopped"));
}

bool FMCPHttpServer::HandlePostRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	FString RequestBody;
	if (Request.Body.Num() > 0)
	{
		FUTF8ToTCHAR Converter(reinterpret_cast<const ANSICHAR*>(Request.Body.GetData()), Request.Body.Num());
		RequestBody = FString(Converter.Length(), Converter.Get());
	}
	if (RequestBody.IsEmpty())
	{
		auto Response = FHttpServerResponse::Error(
			EHttpServerResponseCodes::BadRequest,
			TEXT("EmptyBody"),
			TEXT("Request body is empty"));
		OnComplete(MoveTemp(Response));
		return true;
	}

	FString SessionId;
	const TArray<FString>* SessionHeaders = Request.Headers.Find(TEXT("Mcp-Session-Id"));
	if (SessionHeaders && SessionHeaders->Num() > 0)
	{
		SessionId = (*SessionHeaders)[0];
	}

	FString ResponseBody;
	FString NewSessionId;
	int32 HttpCode = 200;
	JsonRpc.ProcessMessage(RequestBody, SessionId, ResponseBody, NewSessionId, HttpCode);

	if (HttpCode == 202)
	{
		auto Response = FHttpServerResponse::Ok();
		Response->Code = EHttpServerResponseCodes::Accepted;
		OnComplete(MoveTemp(Response));
		return true;
	}

	auto Response = FHttpServerResponse::Create(ResponseBody, TEXT("application/json"));
	Response->Code = static_cast<EHttpServerResponseCodes>(HttpCode);

	if (!NewSessionId.IsEmpty())
	{
		Response->Headers.Add(TEXT("Mcp-Session-Id"), { NewSessionId });
	}

	OnComplete(MoveTemp(Response));
	return true;
}

bool FMCPHttpServer::HandleDeleteRequest(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	FString SessionId;
	const TArray<FString>* SessionHeaders = Request.Headers.Find(TEXT("Mcp-Session-Id"));
	if (SessionHeaders && SessionHeaders->Num() > 0)
	{
		SessionId = (*SessionHeaders)[0];
	}

	if (SessionId.IsEmpty() || !SessionManager.IsValidSession(SessionId))
	{
		auto Response = FHttpServerResponse::Error(
			EHttpServerResponseCodes::NotFound,
			TEXT("InvalidSession"),
			TEXT("Session not found"));
		OnComplete(MoveTemp(Response));
		return true;
	}

	SessionManager.RemoveSession(SessionId);

	auto Response = FHttpServerResponse::Ok();
	OnComplete(MoveTemp(Response));
	return true;
}
