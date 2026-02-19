// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddCommentBoxImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"
#include "Math/Color.h"

FAddCommentBoxImplTool::FAddCommentBoxImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FAddCommentBoxImplTool::GetName() const
{
	return TEXT("add_comment_box");
}

FString FAddCommentBoxImplTool::GetDescription() const
{
	return TEXT("Add a Comment Box to a Blueprint graph. Optionally wraps specified nodes.");
}

TSharedPtr<FJsonObject> FAddCommentBoxImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> CommentTextProp = MakeShared<FJsonObject>();
	CommentTextProp->SetStringField(TEXT("type"), TEXT("string"));
	CommentTextProp->SetStringField(TEXT("description"), TEXT("Text displayed in the comment box"));
	Properties->SetObjectField(TEXT("comment_text"), CommentTextProp);

	// Optional position
	TSharedPtr<FJsonObject> PositionProp = MakeShared<FJsonObject>();
	PositionProp->SetStringField(TEXT("type"), TEXT("object"));
	PositionProp->SetStringField(TEXT("description"), TEXT("Position of the comment box {x, y}. Default (0, 0)."));
	TSharedPtr<FJsonObject> PosProps = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));
	PosProps->SetObjectField(TEXT("x"), NumProp);
	PosProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	PositionProp->SetObjectField(TEXT("properties"), PosProps);
	Properties->SetObjectField(TEXT("position"), PositionProp);

	// Optional size
	TSharedPtr<FJsonObject> SizeProp = MakeShared<FJsonObject>();
	SizeProp->SetStringField(TEXT("type"), TEXT("object"));
	SizeProp->SetStringField(TEXT("description"), TEXT("Size of the comment box {width, height}. Default (400, 200)."));
	TSharedPtr<FJsonObject> SizeProps = MakeShared<FJsonObject>();
	SizeProps->SetObjectField(TEXT("width"), MakeShared<FJsonObject>(*NumProp));
	SizeProps->SetObjectField(TEXT("height"), MakeShared<FJsonObject>(*NumProp));
	SizeProp->SetObjectField(TEXT("properties"), SizeProps);
	Properties->SetObjectField(TEXT("size"), SizeProp);

	// Optional color
	TSharedPtr<FJsonObject> ColorProp = MakeShared<FJsonObject>();
	ColorProp->SetStringField(TEXT("type"), TEXT("object"));
	ColorProp->SetStringField(TEXT("description"), TEXT("Color of the comment box {r, g, b, a} as 0.0-1.0 floats."));
	TSharedPtr<FJsonObject> ColorProps = MakeShared<FJsonObject>();
	ColorProps->SetObjectField(TEXT("r"), MakeShared<FJsonObject>(*NumProp));
	ColorProps->SetObjectField(TEXT("g"), MakeShared<FJsonObject>(*NumProp));
	ColorProps->SetObjectField(TEXT("b"), MakeShared<FJsonObject>(*NumProp));
	ColorProps->SetObjectField(TEXT("a"), MakeShared<FJsonObject>(*NumProp));
	ColorProp->SetObjectField(TEXT("properties"), ColorProps);
	Properties->SetObjectField(TEXT("color"), ColorProp);

	// Optional node_ids
	TSharedPtr<FJsonObject> NodeIdsProp = MakeShared<FJsonObject>();
	NodeIdsProp->SetStringField(TEXT("type"), TEXT("array"));
	NodeIdsProp->SetStringField(TEXT("description"), TEXT("Array of node IDs to wrap with the comment box"));
	TSharedPtr<FJsonObject> NodeIdItemProp = MakeShared<FJsonObject>();
	NodeIdItemProp->SetStringField(TEXT("type"), TEXT("string"));
	NodeIdsProp->SetObjectField(TEXT("items"), NodeIdItemProp);
	Properties->SetObjectField(TEXT("node_ids"), NodeIdsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("comment_text")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddCommentBoxImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString CommentText;
	if (!Arguments->TryGetStringField(TEXT("comment_text"), CommentText))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: comment_text"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional position
	double PosXd = 0.0, PosYd = 0.0;
	const TSharedPtr<FJsonObject>* PositionObj;
	if (Arguments->TryGetObjectField(TEXT("position"), PositionObj))
	{
		(*PositionObj)->TryGetNumberField(TEXT("x"), PosXd);
		(*PositionObj)->TryGetNumberField(TEXT("y"), PosYd);
	}
	int32 PosX = static_cast<int32>(PosXd);
	int32 PosY = static_cast<int32>(PosYd);

	// Parse optional size
	double Widthd = 400.0, Heightd = 200.0;
	const TSharedPtr<FJsonObject>* SizeObj;
	if (Arguments->TryGetObjectField(TEXT("size"), SizeObj))
	{
		(*SizeObj)->TryGetNumberField(TEXT("width"), Widthd);
		(*SizeObj)->TryGetNumberField(TEXT("height"), Heightd);
	}
	int32 Width = static_cast<int32>(Widthd);
	int32 Height = static_cast<int32>(Heightd);

	// Parse optional color
	FLinearColor Color;
	FLinearColor* ColorPtr = nullptr;
	const TSharedPtr<FJsonObject>* ColorObj;
	if (Arguments->TryGetObjectField(TEXT("color"), ColorObj))
	{
		double R = 1.0, G = 1.0, B = 1.0, A = 1.0;
		(*ColorObj)->TryGetNumberField(TEXT("r"), R);
		(*ColorObj)->TryGetNumberField(TEXT("g"), G);
		(*ColorObj)->TryGetNumberField(TEXT("b"), B);
		(*ColorObj)->TryGetNumberField(TEXT("a"), A);
		Color = FLinearColor(static_cast<float>(R), static_cast<float>(G), static_cast<float>(B), static_cast<float>(A));
		ColorPtr = &Color;
	}

	// Parse optional node_ids
	TArray<FString> NodeIds;
	TArray<FString>* NodeIdsPtr = nullptr;
	const TArray<TSharedPtr<FJsonValue>>* NodeIdsArray;
	if (Arguments->TryGetArrayField(TEXT("node_ids"), NodeIdsArray))
	{
		for (const TSharedPtr<FJsonValue>& Value : *NodeIdsArray)
		{
			FString Str;
			if (Value->TryGetString(Str))
			{
				NodeIds.Add(Str);
			}
		}
		NodeIdsPtr = &NodeIds;
	}

	FAddCommentBoxResult BoxResult = BlueprintModule.AddCommentBox(
		BlueprintPath, GraphName, CommentText, PosX, PosY, Width, Height, ColorPtr, NodeIdsPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BoxResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Comment box created with ID: %s"), *BoxResult.NodeId);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add comment box: %s"), *BoxResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
