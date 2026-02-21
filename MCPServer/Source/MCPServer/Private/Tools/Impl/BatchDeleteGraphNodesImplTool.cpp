// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchDeleteGraphNodesImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FBatchDeleteGraphNodesImplTool::FBatchDeleteGraphNodesImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FBatchDeleteGraphNodesImplTool::GetName() const
{
	return TEXT("batch_delete_graph_nodes");
}

FString FBatchDeleteGraphNodesImplTool::GetDescription() const
{
	return TEXT("Delete multiple graph nodes in a single call (up to 100 nodes per call)");
}

TSharedPtr<FJsonObject> FBatchDeleteGraphNodesImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> NodeIdsProp = MakeShared<FJsonObject>();
	NodeIdsProp->SetStringField(TEXT("type"), TEXT("array"));
	NodeIdsProp->SetStringField(TEXT("description"), TEXT("Array of node GUID strings to delete (max 100)"));
	Properties->SetObjectField(TEXT("node_ids"), NodeIdsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("node_ids")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchDeleteGraphNodesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path, graph_name, node_ids"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	const TArray<TSharedPtr<FJsonValue>>* NodeIdsArray = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("node_ids"), NodeIdsArray) || !NodeIdsArray || NodeIdsArray->Num() == 0)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing or empty required parameter: node_ids"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (NodeIdsArray->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Maximum 100 node_ids per call"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	TArray<FString> Errors;

	for (int32 i = 0; i < NodeIdsArray->Num(); ++i)
	{
		FString NodeId;
		if (!(*NodeIdsArray)[i].IsValid() || !(*NodeIdsArray)[i]->TryGetString(NodeId) || NodeId.IsEmpty())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Invalid or empty node_id"), i));
			continue;
		}

		FDeleteGraphNodeResult DeleteResult = BlueprintModule.DeleteGraphNode(BlueprintPath, GraphName, NodeId);
		if (DeleteResult.bSuccess)
		{
			++Succeeded;
		}
		else
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] %s"), i, *DeleteResult.ErrorMessage));
		}
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	FString ResponseText = FString::Printf(TEXT("Batch delete: %d succeeded, %d failed"), Succeeded, Failed);
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
