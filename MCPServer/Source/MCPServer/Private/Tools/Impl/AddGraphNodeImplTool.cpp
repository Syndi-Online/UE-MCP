// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddGraphNodeImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FAddGraphNodeImplTool::FAddGraphNodeImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FAddGraphNodeImplTool::GetName() const
{
	return TEXT("add_graph_node");
}

FString FAddGraphNodeImplTool::GetDescription() const
{
	return TEXT("Add a node to a Blueprint graph");
}

TSharedPtr<FJsonObject> FAddGraphNodeImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> NodeTypeProp = MakeShared<FJsonObject>();
	NodeTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	NodeTypeProp->SetStringField(TEXT("description"), TEXT("Type of node: CallFunction, Event, ComponentBoundEvent, VariableGet, VariableSet, DynamicCast, IfThenElse/Branch, MacroInstance, SwitchEnum, MapForEach, FormatText, EnumToString/GetEnumeratorNameAsString"));
	Properties->SetObjectField(TEXT("node_type"), NodeTypeProp);

	TSharedPtr<FJsonObject> MemberNameProp = MakeShared<FJsonObject>();
	MemberNameProp->SetStringField(TEXT("type"), TEXT("string"));
	MemberNameProp->SetStringField(TEXT("description"), TEXT("Function/variable/macro name. Required for: CallFunction, Event, VariableGet, VariableSet, MacroInstance. E.g. PrintString, ForEachLoop"));
	Properties->SetObjectField(TEXT("member_name"), MemberNameProp);

	TSharedPtr<FJsonObject> TargetProp = MakeShared<FJsonObject>();
	TargetProp->SetStringField(TEXT("type"), TEXT("string"));
	TargetProp->SetStringField(TEXT("description"), TEXT("Target class/type. For CallFunction: class name. For DynamicCast: class to cast to. For SwitchEnum: enum path. For ComponentBoundEvent: component variable name"));
	Properties->SetObjectField(TEXT("target"), TargetProp);

	TSharedPtr<FJsonObject> PositionProp = MakeShared<FJsonObject>();
	PositionProp->SetStringField(TEXT("type"), TEXT("object"));
	PositionProp->SetStringField(TEXT("description"), TEXT("Optional position of the node {x, y}"));
	TSharedPtr<FJsonObject> PosProps = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));
	PosProps->SetObjectField(TEXT("x"), NumProp);
	PosProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	PositionProp->SetObjectField(TEXT("properties"), PosProps);
	Properties->SetObjectField(TEXT("position"), PositionProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("node_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddGraphNodeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString NodeType;
	if (!Arguments->TryGetStringField(TEXT("node_type"), NodeType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: node_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional member_name
	FString MemberName;
	bool bHasMemberName = Arguments->TryGetStringField(TEXT("member_name"), MemberName);
	const FString* MemberNamePtr = bHasMemberName ? &MemberName : nullptr;

	// Parse optional target
	FString Target;
	bool bHasTarget = Arguments->TryGetStringField(TEXT("target"), Target);
	const FString* TargetPtr = bHasTarget ? &Target : nullptr;

	// Parse optional position
	int32 PosXVal = 0;
	int32 PosYVal = 0;
	bool bHasPosition = false;
	const TSharedPtr<FJsonObject>* PositionObj;
	if (Arguments->TryGetObjectField(TEXT("position"), PositionObj))
	{
		double PosXd = 0.0, PosYd = 0.0;
		(*PositionObj)->TryGetNumberField(TEXT("x"), PosXd);
		(*PositionObj)->TryGetNumberField(TEXT("y"), PosYd);
		PosXVal = static_cast<int32>(PosXd);
		PosYVal = static_cast<int32>(PosYd);
		bHasPosition = true;
	}
	const int32* PosXPtr = bHasPosition ? &PosXVal : nullptr;
	const int32* PosYPtr = bHasPosition ? &PosYVal : nullptr;

	FAddGraphNodeResult NodeResult = BlueprintModule.AddGraphNode(
		BlueprintPath, GraphName, NodeType, MemberNamePtr, TargetPtr, PosXPtr, PosYPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (NodeResult.bSuccess)
	{
		TSharedPtr<FJsonObject> NodeObj = MakeShared<FJsonObject>();
		NodeObj->SetStringField(TEXT("node_id"), NodeResult.NodeId);

		TArray<TSharedPtr<FJsonValue>> PinsArray;
		for (const FGraphNodePinInfo& PinInfo : NodeResult.Pins)
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

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(NodeObj, Writer);

		FString ResponseText = FString::Printf(
			TEXT("Node added with ID: %s and %d pins.\n%s"),
			*NodeResult.NodeId, NodeResult.Pins.Num(), *JsonString);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add graph node: %s"), *NodeResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
