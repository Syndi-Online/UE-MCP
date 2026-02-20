// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DisconnectGraphPinsImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FDisconnectGraphPinsImplTool::FDisconnectGraphPinsImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FDisconnectGraphPinsImplTool::GetName() const
{
	return TEXT("disconnect_graph_pins");
}

FString FDisconnectGraphPinsImplTool::GetDescription() const
{
	return TEXT("Disconnect two connected pins in a Blueprint graph");
}

TSharedPtr<FJsonObject> FDisconnectGraphPinsImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> SourceNodeIdProp = MakeShared<FJsonObject>();
	SourceNodeIdProp->SetStringField(TEXT("type"), TEXT("string"));
	SourceNodeIdProp->SetStringField(TEXT("description"), TEXT("Node ID of the source node"));
	Properties->SetObjectField(TEXT("source_node_id"), SourceNodeIdProp);

	TSharedPtr<FJsonObject> SourcePinNameProp = MakeShared<FJsonObject>();
	SourcePinNameProp->SetStringField(TEXT("type"), TEXT("string"));
	SourcePinNameProp->SetStringField(TEXT("description"), TEXT("Pin name on the source node"));
	Properties->SetObjectField(TEXT("source_pin_name"), SourcePinNameProp);

	TSharedPtr<FJsonObject> TargetNodeIdProp = MakeShared<FJsonObject>();
	TargetNodeIdProp->SetStringField(TEXT("type"), TEXT("string"));
	TargetNodeIdProp->SetStringField(TEXT("description"), TEXT("Node ID of the target node"));
	Properties->SetObjectField(TEXT("target_node_id"), TargetNodeIdProp);

	TSharedPtr<FJsonObject> TargetPinNameProp = MakeShared<FJsonObject>();
	TargetPinNameProp->SetStringField(TEXT("type"), TEXT("string"));
	TargetPinNameProp->SetStringField(TEXT("description"), TEXT("Pin name on the target node"));
	Properties->SetObjectField(TEXT("target_pin_name"), TargetPinNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("source_node_id")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("source_pin_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("target_node_id")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("target_pin_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FDisconnectGraphPinsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString SourceNodeId;
	if (!Arguments->TryGetStringField(TEXT("source_node_id"), SourceNodeId))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: source_node_id"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString SourcePinName;
	if (!Arguments->TryGetStringField(TEXT("source_pin_name"), SourcePinName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: source_pin_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString TargetNodeId;
	if (!Arguments->TryGetStringField(TEXT("target_node_id"), TargetNodeId))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: target_node_id"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString TargetPinName;
	if (!Arguments->TryGetStringField(TEXT("target_pin_name"), TargetPinName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: target_pin_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FDisconnectGraphPinsResult PinResult = BlueprintModule.DisconnectGraphPins(
		BlueprintPath, GraphName, SourceNodeId, SourcePinName, TargetNodeId, TargetPinName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PinResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Disconnected %s.%s from %s.%s"),
			*SourceNodeId, *SourcePinName, *TargetNodeId, *TargetPinName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to disconnect pins: %s"), *PinResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
