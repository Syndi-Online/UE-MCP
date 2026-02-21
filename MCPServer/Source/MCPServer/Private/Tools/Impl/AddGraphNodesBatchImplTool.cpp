// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddGraphNodesBatchImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FAddGraphNodesBatchImplTool::FAddGraphNodesBatchImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FAddGraphNodesBatchImplTool::GetName() const
{
	return TEXT("add_graph_nodes_batch");
}

FString FAddGraphNodesBatchImplTool::GetDescription() const
{
	return TEXT("Atomically create multiple graph nodes with pin defaults and connections in a single call. On failure, all created nodes are rolled back.");
}

TSharedPtr<FJsonObject> FAddGraphNodesBatchImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> GraphNameProp = MakeShared<FJsonObject>();
	GraphNameProp->SetStringField(TEXT("type"), TEXT("string"));
	GraphNameProp->SetStringField(TEXT("description"), TEXT("Name of the graph"));
	Properties->SetObjectField(TEXT("graph_name"), GraphNameProp);

	// Nodes array
	TSharedPtr<FJsonObject> NodesProp = MakeShared<FJsonObject>();
	NodesProp->SetStringField(TEXT("type"), TEXT("array"));
	NodesProp->SetStringField(TEXT("description"), TEXT("Array of nodes to create. Each: {local_id, node_type, member_name, target, pos_x, pos_y, pin_defaults: {pin_name: value}}"));
	Properties->SetObjectField(TEXT("nodes"), NodesProp);

	// Connections array
	TSharedPtr<FJsonObject> ConnsProp = MakeShared<FJsonObject>();
	ConnsProp->SetStringField(TEXT("type"), TEXT("array"));
	ConnsProp->SetStringField(TEXT("description"), TEXT("Array of connections: {source, source_pin, target, target_pin}. Source/target can be local_ids or existing node GUIDs."));
	Properties->SetObjectField(TEXT("connections"), ConnsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("nodes")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddGraphNodesBatchImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path, graph_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse nodes array
	const TArray<TSharedPtr<FJsonValue>>* NodesArray = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("nodes"), NodesArray) || !NodesArray || NodesArray->Num() == 0)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing or empty required parameter: nodes"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	TArray<FAddGraphNodesBatchNodeInfo> Nodes;
	for (const TSharedPtr<FJsonValue>& NodeVal : *NodesArray)
	{
		const TSharedPtr<FJsonObject>* NodeObj = nullptr;
		if (!NodeVal.IsValid() || !NodeVal->TryGetObject(NodeObj) || !NodeObj || !(*NodeObj).IsValid()) continue;

		FAddGraphNodesBatchNodeInfo Info;
		(*NodeObj)->TryGetStringField(TEXT("local_id"), Info.LocalId);
		(*NodeObj)->TryGetStringField(TEXT("node_type"), Info.NodeType);
		(*NodeObj)->TryGetStringField(TEXT("member_name"), Info.MemberName);
		(*NodeObj)->TryGetStringField(TEXT("target"), Info.Target);

		double PosXD = 0, PosYD = 0;
		if ((*NodeObj)->TryGetNumberField(TEXT("pos_x"), PosXD)) Info.PosX = static_cast<int32>(PosXD);
		if ((*NodeObj)->TryGetNumberField(TEXT("pos_y"), PosYD)) Info.PosY = static_cast<int32>(PosYD);

		if (!Info.LocalId.IsEmpty() && !Info.NodeType.IsEmpty())
		{
			Nodes.Add(Info);
		}
	}

	// Parse connections array
	TArray<FAddGraphNodesBatchConnection> Connections;
	const TArray<TSharedPtr<FJsonValue>>* ConnsArray = nullptr;
	if (Arguments->TryGetArrayField(TEXT("connections"), ConnsArray) && ConnsArray)
	{
		for (const TSharedPtr<FJsonValue>& ConnVal : *ConnsArray)
		{
			const TSharedPtr<FJsonObject>* ConnObj = nullptr;
			if (!ConnVal.IsValid() || !ConnVal->TryGetObject(ConnObj) || !ConnObj || !(*ConnObj).IsValid()) continue;

			FAddGraphNodesBatchConnection Conn;
			(*ConnObj)->TryGetStringField(TEXT("source"), Conn.SourceLocalId);
			(*ConnObj)->TryGetStringField(TEXT("source_pin"), Conn.SourcePinName);
			(*ConnObj)->TryGetStringField(TEXT("target"), Conn.TargetLocalId);
			(*ConnObj)->TryGetStringField(TEXT("target_pin"), Conn.TargetPinName);
			Connections.Add(Conn);
		}
	}

	FAddGraphNodesBatchResult BatchResult = BlueprintModule.AddGraphNodesBatch(BlueprintPath, GraphName, Nodes, Connections.Num() > 0 ? &Connections : nullptr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BatchResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Created %d nodes, %d connections:\n"),
			BatchResult.Nodes.Num(), BatchResult.ConnectionsMade);
		for (const FAddGraphNodesBatchResultNode& N : BatchResult.Nodes)
		{
			ResponseText += FString::Printf(TEXT("- %s -> node_id: %s (%d pins)\n"),
				*N.LocalId, *N.NodeId, N.Pins.Num());
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Batch failed: %s"), *BatchResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
