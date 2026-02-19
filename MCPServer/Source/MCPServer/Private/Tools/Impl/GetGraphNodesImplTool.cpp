// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetGraphNodesImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FGetGraphNodesImplTool::FGetGraphNodesImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FGetGraphNodesImplTool::GetName() const
{
	return TEXT("get_graph_nodes");
}

FString FGetGraphNodesImplTool::GetDescription() const
{
	return TEXT("Get all nodes in a Blueprint graph with their id, class, title, position, size, pins, and comment text");
}

TSharedPtr<FJsonObject> FGetGraphNodesImplTool::GetInputSchema() const
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

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetGraphNodesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FGetGraphNodesResult NodesResult = BlueprintModule.GetGraphNodes(BlueprintPath, GraphName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (NodesResult.bSuccess)
	{
		TArray<TSharedPtr<FJsonValue>> NodesArray;
		for (const FGraphNodeInfo& Info : NodesResult.Nodes)
		{
			TSharedPtr<FJsonObject> NodeObj = MakeShared<FJsonObject>();
			NodeObj->SetStringField(TEXT("node_id"), Info.NodeId);
			NodeObj->SetStringField(TEXT("node_class"), Info.NodeClass);
			NodeObj->SetStringField(TEXT("node_title"), Info.NodeTitle);

			TSharedPtr<FJsonObject> PosObj = MakeShared<FJsonObject>();
			PosObj->SetNumberField(TEXT("x"), Info.PosX);
			PosObj->SetNumberField(TEXT("y"), Info.PosY);
			NodeObj->SetObjectField(TEXT("position"), PosObj);

			TSharedPtr<FJsonObject> SizeObj = MakeShared<FJsonObject>();
			SizeObj->SetNumberField(TEXT("width"), Info.Width);
			SizeObj->SetNumberField(TEXT("height"), Info.Height);
			NodeObj->SetObjectField(TEXT("size"), SizeObj);

			NodeObj->SetStringField(TEXT("comment"), Info.Comment);

			TArray<TSharedPtr<FJsonValue>> PinsArray;
			for (const FGraphNodePinInfo& PinInfo : Info.Pins)
			{
				TSharedPtr<FJsonObject> PinObj = MakeShared<FJsonObject>();
				PinObj->SetStringField(TEXT("pin_id"), PinInfo.PinId);
				PinObj->SetStringField(TEXT("pin_name"), PinInfo.PinName);
				PinObj->SetStringField(TEXT("pin_type"), PinInfo.PinType);
				PinObj->SetStringField(TEXT("direction"), PinInfo.Direction);

				TArray<TSharedPtr<FJsonValue>> ConnectedArray;
				for (const FString& ConnectedId : PinInfo.ConnectedPinIds)
				{
					ConnectedArray.Add(MakeShared<FJsonValueString>(ConnectedId));
				}
				PinObj->SetArrayField(TEXT("connected_pin_ids"), ConnectedArray);

				PinsArray.Add(MakeShared<FJsonValueObject>(PinObj));
			}
			NodeObj->SetArrayField(TEXT("pins"), PinsArray);

			NodesArray.Add(MakeShared<FJsonValueObject>(NodeObj));
		}

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(NodesArray, Writer);

		FString ResponseText = FString::Printf(
			TEXT("Graph has %d nodes.\n%s"),
			NodesResult.Nodes.Num(),
			*JsonString);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get graph nodes: %s"), *NodesResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
