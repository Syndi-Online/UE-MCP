// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetNodePositionImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FSetNodePositionImplTool::FSetNodePositionImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FSetNodePositionImplTool::GetName() const
{
	return TEXT("set_node_position");
}

FString FSetNodePositionImplTool::GetDescription() const
{
	return TEXT("Move a node to a new position on the Blueprint graph");
}

TSharedPtr<FJsonObject> FSetNodePositionImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> NodeIdProp = MakeShared<FJsonObject>();
	NodeIdProp->SetStringField(TEXT("type"), TEXT("string"));
	NodeIdProp->SetStringField(TEXT("description"), TEXT("ID of the node to move"));
	Properties->SetObjectField(TEXT("node_id"), NodeIdProp);

	TSharedPtr<FJsonObject> PositionProp = MakeShared<FJsonObject>();
	PositionProp->SetStringField(TEXT("type"), TEXT("object"));
	PositionProp->SetStringField(TEXT("description"), TEXT("New position {x, y}"));
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
	Required.Add(MakeShared<FJsonValueString>(TEXT("node_id")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("position")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetNodePositionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	const TSharedPtr<FJsonObject>* PositionObj;
	if (!Arguments->TryGetObjectField(TEXT("position"), PositionObj))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: position"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	double PosXd = 0.0, PosYd = 0.0;
	(*PositionObj)->TryGetNumberField(TEXT("x"), PosXd);
	(*PositionObj)->TryGetNumberField(TEXT("y"), PosYd);
	int32 PosX = static_cast<int32>(PosXd);
	int32 PosY = static_cast<int32>(PosYd);

	FSetNodePositionResult PosResult = BlueprintModule.SetNodePosition(BlueprintPath, GraphName, NodeId, PosX, PosY);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PosResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Node moved to (%d, %d)"), PosX, PosY);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set node position: %s"), *PosResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
