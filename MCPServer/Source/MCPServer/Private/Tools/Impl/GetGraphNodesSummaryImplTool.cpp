// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetGraphNodesSummaryImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FGetGraphNodesSummaryImplTool::FGetGraphNodesSummaryImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FGetGraphNodesSummaryImplTool::GetName() const
{
	return TEXT("get_graph_nodes_summary");
}

FString FGetGraphNodesSummaryImplTool::GetDescription() const
{
	return TEXT("Get a lightweight summary of all nodes in a Blueprint graph — id, class, title, and connected node ids. No pins, no positions, no sizes.");
}

TSharedPtr<FJsonObject> FGetGraphNodesSummaryImplTool::GetInputSchema() const
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
	GraphNameProp->SetStringField(TEXT("description"), TEXT("Name of the graph within the Blueprint"));
	Properties->SetObjectField(TEXT("graph_name"), GraphNameProp);

	TSharedPtr<FJsonObject> ClassFilterProp = MakeShared<FJsonObject>();
	ClassFilterProp->SetStringField(TEXT("type"), TEXT("string"));
	ClassFilterProp->SetStringField(TEXT("description"), TEXT("If specified, return only nodes of this class"));
	Properties->SetObjectField(TEXT("class_filter"), ClassFilterProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetGraphNodesSummaryImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: blueprint_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString GraphName;
	if (!Arguments->TryGetStringField(TEXT("graph_name"), GraphName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: graph_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ClassFilter;
	Arguments->TryGetStringField(TEXT("class_filter"), ClassFilter);

	FGetGraphNodesResult NodesResult = BlueprintModule.GetGraphNodes(BlueprintPath, GraphName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (NodesResult.bSuccess)
	{
		const int32 TotalNodeCount = NodesResult.Nodes.Num();

		// Collect connected node IDs per node from pin data
		TArray<TSharedPtr<FJsonValue>> NodesArray;
		for (const FGraphNodeInfo& Info : NodesResult.Nodes)
		{
			if (!ClassFilter.IsEmpty() && Info.NodeClass != ClassFilter)
			{
				continue;
			}

			TSharedPtr<FJsonObject> NodeObj = MakeShared<FJsonObject>();
			NodeObj->SetStringField(TEXT("node_id"), Info.NodeId);
			NodeObj->SetStringField(TEXT("node_class"), Info.NodeClass);
			NodeObj->SetStringField(TEXT("node_title"), Info.NodeTitle);

			// Collect unique connected node IDs from all pins
			TSet<FString> ConnectedNodeIdSet;
			for (const FGraphNodePinInfo& PinInfo : Info.Pins)
			{
				for (const FString& ConnectedPinId : PinInfo.ConnectedPinIds)
				{
					// Find which node owns this pin
					for (const FGraphNodeInfo& OtherNode : NodesResult.Nodes)
					{
						if (OtherNode.NodeId == Info.NodeId) continue;
						for (const FGraphNodePinInfo& OtherPin : OtherNode.Pins)
						{
							if (OtherPin.PinId == ConnectedPinId)
							{
								ConnectedNodeIdSet.Add(OtherNode.NodeId);
							}
						}
					}
				}
			}

			TArray<TSharedPtr<FJsonValue>> ConnectedArray;
			for (const FString& ConnectedNodeId : ConnectedNodeIdSet)
			{
				ConnectedArray.Add(MakeShared<FJsonValueString>(ConnectedNodeId));
			}
			NodeObj->SetArrayField(TEXT("connected_node_ids"), ConnectedArray);

			NodesArray.Add(MakeShared<FJsonValueObject>(NodeObj));
		}

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(NodesArray, Writer);

		const int32 FilteredCount = NodesArray.Num();
		FString ResponseText;
		if (ClassFilter.IsEmpty())
		{
			ResponseText = FString::Printf(TEXT("Graph has %d nodes.\n%s"), TotalNodeCount, *JsonString);
		}
		else
		{
			ResponseText = FString::Printf(TEXT("Graph has %d nodes (filtered from %d total).\n%s"), FilteredCount, TotalNodeCount, *JsonString);
		}

		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get graph nodes summary: %s"), *NodesResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
