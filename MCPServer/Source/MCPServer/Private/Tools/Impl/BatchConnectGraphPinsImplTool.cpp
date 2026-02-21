// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchConnectGraphPinsImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FBatchConnectGraphPinsImplTool::FBatchConnectGraphPinsImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FBatchConnectGraphPinsImplTool::GetName() const
{
	return TEXT("batch_connect_graph_pins");
}

FString FBatchConnectGraphPinsImplTool::GetDescription() const
{
	return TEXT("Connect multiple pin pairs in a single call (up to 100 connections per call)");
}

TSharedPtr<FJsonObject> FBatchConnectGraphPinsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BPProp = MakeShared<FJsonObject>();
	BPProp->SetStringField(TEXT("type"), TEXT("string"));
	BPProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BPProp);

	TSharedPtr<FJsonObject> GraphProp = MakeShared<FJsonObject>();
	GraphProp->SetStringField(TEXT("type"), TEXT("string"));
	GraphProp->SetStringField(TEXT("description"), TEXT("Name of the graph"));
	Properties->SetObjectField(TEXT("graph_name"), GraphProp);

	TSharedPtr<FJsonObject> ConnsProp = MakeShared<FJsonObject>();
	ConnsProp->SetStringField(TEXT("type"), TEXT("array"));
	ConnsProp->SetStringField(TEXT("description"), TEXT("Array of connections (max 100). Each: {source_node_id, source_pin, target_node_id, target_pin}"));
	Properties->SetObjectField(TEXT("connections"), ConnsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("connections")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchConnectGraphPinsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath, GraphName;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath) ||
		!Arguments->TryGetStringField(TEXT("graph_name"), GraphName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path, graph_name, connections"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	const TArray<TSharedPtr<FJsonValue>>* ConnsArray = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("connections"), ConnsArray) || !ConnsArray || ConnsArray->Num() == 0)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing or empty required parameter: connections"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (ConnsArray->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Maximum 100 connections per call"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	TArray<FString> Errors;

	for (int32 i = 0; i < ConnsArray->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* ConnObj = nullptr;
		if (!(*ConnsArray)[i].IsValid() || !(*ConnsArray)[i]->TryGetObject(ConnObj) || !ConnObj || !(*ConnObj).IsValid())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Invalid connection object"), i));
			continue;
		}

		FString SourceNodeId, SourcePin, TargetNodeId, TargetPin;
		(*ConnObj)->TryGetStringField(TEXT("source_node_id"), SourceNodeId);
		(*ConnObj)->TryGetStringField(TEXT("source_pin"), SourcePin);
		(*ConnObj)->TryGetStringField(TEXT("target_node_id"), TargetNodeId);
		(*ConnObj)->TryGetStringField(TEXT("target_pin"), TargetPin);

		if (SourceNodeId.IsEmpty() || SourcePin.IsEmpty() || TargetNodeId.IsEmpty() || TargetPin.IsEmpty())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Missing fields in connection"), i));
			continue;
		}

		FConnectGraphPinsResult ConnResult = BlueprintModule.ConnectGraphPins(BlueprintPath, GraphName, SourceNodeId, SourcePin, TargetNodeId, TargetPin);
		if (ConnResult.bSuccess)
		{
			++Succeeded;
		}
		else
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] %s"), i, *ConnResult.ErrorMessage));
		}
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	FString ResponseText = FString::Printf(TEXT("Batch connect: %d succeeded, %d failed"), Succeeded, Failed);
	if (Errors.Num() > 0)
	{
		ResponseText += TEXT("\nErrors:");
		for (const FString& Err : Errors)
		{
			ResponseText += FString::Printf(TEXT("\n- %s"), *Err);
		}
	}
	TextContent->SetStringField(TEXT("text"), ResponseText);

	Result->SetBoolField(TEXT("isError"), Succeeded == 0 && Failed > 0);

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
