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
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("batch_disconnect_graph_pins: not implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
