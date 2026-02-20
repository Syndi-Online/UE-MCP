// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DeleteGraphNodeImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FDeleteGraphNodeImplTool::FDeleteGraphNodeImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FDeleteGraphNodeImplTool::GetName() const
{
	return TEXT("delete_graph_node");
}

FString FDeleteGraphNodeImplTool::GetDescription() const
{
	return TEXT("Delete a node from a Blueprint graph");
}

TSharedPtr<FJsonObject> FDeleteGraphNodeImplTool::GetInputSchema() const
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
	GraphNameProp->SetStringField(TEXT("description"), TEXT("Name of the graph (e.g. EventGraph)"));
	Properties->SetObjectField(TEXT("graph_name"), GraphNameProp);

	TSharedPtr<FJsonObject> NodeIdProp = MakeShared<FJsonObject>();
	NodeIdProp->SetStringField(TEXT("type"), TEXT("string"));
	NodeIdProp->SetStringField(TEXT("description"), TEXT("Node ID (GUID) of the node to delete"));
	Properties->SetObjectField(TEXT("node_id"), NodeIdProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("node_id")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FDeleteGraphNodeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString NodeId;
	if (!Arguments->TryGetStringField(TEXT("node_id"), NodeId))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: node_id"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FDeleteGraphNodeResult DelResult = BlueprintModule.DeleteGraphNode(BlueprintPath, GraphName, NodeId);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DelResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Node %s deleted from graph '%s'"), *NodeId, *GraphName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to delete node: %s"), *DelResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
