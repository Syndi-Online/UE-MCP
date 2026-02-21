// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchDisconnectGraphPinsImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FBatchDisconnectGraphPinsImplTool::FBatchDisconnectGraphPinsImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FBatchDisconnectGraphPinsImplTool::GetName() const
{
	return TEXT("batch_disconnect_graph_pins");
}

FString FBatchDisconnectGraphPinsImplTool::GetDescription() const
{
	return TEXT("Disconnect multiple pin pairs in a single call (up to 100 disconnections per call)");
}

TSharedPtr<FJsonObject> FBatchDisconnectGraphPinsImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> DisconnsProp = MakeShared<FJsonObject>();
	DisconnsProp->SetStringField(TEXT("type"), TEXT("array"));
	DisconnsProp->SetStringField(TEXT("description"), TEXT("Array of disconnections (max 100). Each: {source_node_id, source_pin, target_node_id, target_pin}"));
	Properties->SetObjectField(TEXT("disconnections"), DisconnsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("disconnections")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchDisconnectGraphPinsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path, graph_name, disconnections"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	const TArray<TSharedPtr<FJsonValue>>* DisconnsArray = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("disconnections"), DisconnsArray) || !DisconnsArray || DisconnsArray->Num() == 0)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing or empty required parameter: disconnections"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (DisconnsArray->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Maximum 100 disconnections per call"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	TArray<FString> Errors;

	for (int32 i = 0; i < DisconnsArray->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* DisconnObj = nullptr;
		if (!(*DisconnsArray)[i].IsValid() || !(*DisconnsArray)[i]->TryGetObject(DisconnObj) || !DisconnObj || !(*DisconnObj).IsValid())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Invalid disconnection object"), i));
			continue;
		}

		FString SourceNodeId, SourcePin, TargetNodeId, TargetPin;
		(*DisconnObj)->TryGetStringField(TEXT("source_node_id"), SourceNodeId);
		(*DisconnObj)->TryGetStringField(TEXT("source_pin"), SourcePin);
		(*DisconnObj)->TryGetStringField(TEXT("target_node_id"), TargetNodeId);
		(*DisconnObj)->TryGetStringField(TEXT("target_pin"), TargetPin);

		if (SourceNodeId.IsEmpty() || SourcePin.IsEmpty() || TargetNodeId.IsEmpty() || TargetPin.IsEmpty())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Missing fields in disconnection"), i));
			continue;
		}

		FDisconnectGraphPinsResult DisconnResult = BlueprintModule.DisconnectGraphPins(BlueprintPath, GraphName, SourceNodeId, SourcePin, TargetNodeId, TargetPin);
		if (DisconnResult.bSuccess)
		{
			++Succeeded;
		}
		else
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] %s"), i, *DisconnResult.ErrorMessage));
		}
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	FString ResponseText = FString::Printf(TEXT("Batch disconnect: %d succeeded, %d failed"), Succeeded, Failed);
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
