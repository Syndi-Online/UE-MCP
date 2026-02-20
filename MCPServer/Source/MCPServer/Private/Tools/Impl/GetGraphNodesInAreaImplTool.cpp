// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetGraphNodesInAreaImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FGetGraphNodesInAreaImplTool::FGetGraphNodesInAreaImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FGetGraphNodesInAreaImplTool::GetName() const
{
	return TEXT("get_graph_nodes_in_area");
}

FString FGetGraphNodesInAreaImplTool::GetDescription() const
{
	return TEXT("Query graph nodes that intersect a rectangular area. Returns empty array if area is free.");
}

TSharedPtr<FJsonObject> FGetGraphNodesInAreaImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> MinXProp = MakeShared<FJsonObject>();
	MinXProp->SetStringField(TEXT("type"), TEXT("number"));
	MinXProp->SetStringField(TEXT("description"), TEXT("Left boundary of area"));
	Properties->SetObjectField(TEXT("min_x"), MinXProp);

	TSharedPtr<FJsonObject> MinYProp = MakeShared<FJsonObject>();
	MinYProp->SetStringField(TEXT("type"), TEXT("number"));
	MinYProp->SetStringField(TEXT("description"), TEXT("Top boundary of area"));
	Properties->SetObjectField(TEXT("min_y"), MinYProp);

	TSharedPtr<FJsonObject> MaxXProp = MakeShared<FJsonObject>();
	MaxXProp->SetStringField(TEXT("type"), TEXT("number"));
	MaxXProp->SetStringField(TEXT("description"), TEXT("Right boundary of area"));
	Properties->SetObjectField(TEXT("max_x"), MaxXProp);

	TSharedPtr<FJsonObject> MaxYProp = MakeShared<FJsonObject>();
	MaxYProp->SetStringField(TEXT("type"), TEXT("number"));
	MaxYProp->SetStringField(TEXT("description"), TEXT("Bottom boundary of area"));
	Properties->SetObjectField(TEXT("max_y"), MaxYProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("min_x")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("min_y")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("max_x")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("max_y")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetGraphNodesInAreaImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath, GraphName;
	double MinXD = 0, MinYD = 0, MaxXD = 0, MaxYD = 0;

	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath) ||
		!Arguments->TryGetStringField(TEXT("graph_name"), GraphName) ||
		!Arguments->TryGetNumberField(TEXT("min_x"), MinXD) ||
		!Arguments->TryGetNumberField(TEXT("min_y"), MinYD) ||
		!Arguments->TryGetNumberField(TEXT("max_x"), MaxXD) ||
		!Arguments->TryGetNumberField(TEXT("max_y"), MaxYD))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path, graph_name, min_x, min_y, max_x, max_y"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FGetGraphNodesInAreaResult AreaResult = BlueprintModule.GetGraphNodesInArea(
		BlueprintPath, GraphName,
		static_cast<int32>(MinXD), static_cast<int32>(MinYD),
		static_cast<int32>(MaxXD), static_cast<int32>(MaxYD));

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (AreaResult.bSuccess)
	{
		if (AreaResult.Nodes.Num() == 0)
		{
			TextContent->SetStringField(TEXT("text"), TEXT("Area is free — 0 nodes found."));
		}
		else
		{
			FString ResponseText = FString::Printf(TEXT("Found %d nodes in area:\n"), AreaResult.Nodes.Num());
			for (const FGraphNodeInAreaInfo& N : AreaResult.Nodes)
			{
				ResponseText += FString::Printf(TEXT("- node_id: %s  title: %s  position: (%d, %d)  size: (%d x %d)\n"),
					*N.NodeId, *N.NodeTitle, N.PosX, N.PosY, N.Width, N.Height);
			}
			TextContent->SetStringField(TEXT("text"), ResponseText);
		}
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed: %s"), *AreaResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
