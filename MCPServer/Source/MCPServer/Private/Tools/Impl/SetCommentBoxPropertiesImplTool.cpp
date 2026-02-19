// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetCommentBoxPropertiesImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"
#include "Math/Color.h"

FSetCommentBoxPropertiesImplTool::FSetCommentBoxPropertiesImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FSetCommentBoxPropertiesImplTool::GetName() const
{
	return TEXT("set_comment_box_properties");
}

FString FSetCommentBoxPropertiesImplTool::GetDescription() const
{
	return TEXT("Change properties of a Comment Box (text, color, position, size)");
}

TSharedPtr<FJsonObject> FSetCommentBoxPropertiesImplTool::GetInputSchema() const
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
	NodeIdProp->SetStringField(TEXT("description"), TEXT("Node ID of the comment box to modify"));
	Properties->SetObjectField(TEXT("node_id"), NodeIdProp);

	// Optional comment_text
	TSharedPtr<FJsonObject> CommentTextProp = MakeShared<FJsonObject>();
	CommentTextProp->SetStringField(TEXT("type"), TEXT("string"));
	CommentTextProp->SetStringField(TEXT("description"), TEXT("New text for the comment box"));
	Properties->SetObjectField(TEXT("comment_text"), CommentTextProp);

	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));

	// Optional color
	TSharedPtr<FJsonObject> ColorProp = MakeShared<FJsonObject>();
	ColorProp->SetStringField(TEXT("type"), TEXT("object"));
	ColorProp->SetStringField(TEXT("description"), TEXT("New color {r, g, b, a} as 0.0-1.0 floats."));
	TSharedPtr<FJsonObject> ColorProps = MakeShared<FJsonObject>();
	ColorProps->SetObjectField(TEXT("r"), MakeShared<FJsonObject>(*NumProp));
	ColorProps->SetObjectField(TEXT("g"), MakeShared<FJsonObject>(*NumProp));
	ColorProps->SetObjectField(TEXT("b"), MakeShared<FJsonObject>(*NumProp));
	ColorProps->SetObjectField(TEXT("a"), MakeShared<FJsonObject>(*NumProp));
	ColorProp->SetObjectField(TEXT("properties"), ColorProps);
	Properties->SetObjectField(TEXT("color"), ColorProp);

	// Optional position
	TSharedPtr<FJsonObject> PositionProp = MakeShared<FJsonObject>();
	PositionProp->SetStringField(TEXT("type"), TEXT("object"));
	PositionProp->SetStringField(TEXT("description"), TEXT("New position {x, y}."));
	TSharedPtr<FJsonObject> PosProps = MakeShared<FJsonObject>();
	PosProps->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*NumProp));
	PosProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	PositionProp->SetObjectField(TEXT("properties"), PosProps);
	Properties->SetObjectField(TEXT("position"), PositionProp);

	// Optional size
	TSharedPtr<FJsonObject> SizeProp = MakeShared<FJsonObject>();
	SizeProp->SetStringField(TEXT("type"), TEXT("object"));
	SizeProp->SetStringField(TEXT("description"), TEXT("New size {width, height}."));
	TSharedPtr<FJsonObject> SizeProps = MakeShared<FJsonObject>();
	SizeProps->SetObjectField(TEXT("width"), MakeShared<FJsonObject>(*NumProp));
	SizeProps->SetObjectField(TEXT("height"), MakeShared<FJsonObject>(*NumProp));
	SizeProp->SetObjectField(TEXT("properties"), SizeProps);
	Properties->SetObjectField(TEXT("size"), SizeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("node_id")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetCommentBoxPropertiesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	// Parse optional comment_text
	FString CommentText;
	FString* CommentTextPtr = nullptr;
	if (Arguments->TryGetStringField(TEXT("comment_text"), CommentText))
	{
		CommentTextPtr = &CommentText;
	}

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

	// Parse optional position
	int32 PosX = 0, PosY = 0;
	int32* PosXPtr = nullptr;
	int32* PosYPtr = nullptr;
	const TSharedPtr<FJsonObject>* PositionObj;
	if (Arguments->TryGetObjectField(TEXT("position"), PositionObj))
	{
		double PosXd = 0.0, PosYd = 0.0;
		(*PositionObj)->TryGetNumberField(TEXT("x"), PosXd);
		(*PositionObj)->TryGetNumberField(TEXT("y"), PosYd);
		PosX = static_cast<int32>(PosXd);
		PosY = static_cast<int32>(PosYd);
		PosXPtr = &PosX;
		PosYPtr = &PosY;
	}

	// Parse optional size
	int32 Width = 0, Height = 0;
	int32* WidthPtr = nullptr;
	int32* HeightPtr = nullptr;
	const TSharedPtr<FJsonObject>* SizeObj;
	if (Arguments->TryGetObjectField(TEXT("size"), SizeObj))
	{
		double Widthd = 0.0, Heightd = 0.0;
		(*SizeObj)->TryGetNumberField(TEXT("width"), Widthd);
		(*SizeObj)->TryGetNumberField(TEXT("height"), Heightd);
		Width = static_cast<int32>(Widthd);
		Height = static_cast<int32>(Heightd);
		WidthPtr = &Width;
		HeightPtr = &Height;
	}

	FSetCommentBoxPropertiesResult BoxResult = BlueprintModule.SetCommentBoxProperties(
		BlueprintPath, GraphName, NodeId, CommentTextPtr, ColorPtr, PosXPtr, PosYPtr, WidthPtr, HeightPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BoxResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Comment box properties updated"));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to update comment box properties: %s"), *BoxResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
