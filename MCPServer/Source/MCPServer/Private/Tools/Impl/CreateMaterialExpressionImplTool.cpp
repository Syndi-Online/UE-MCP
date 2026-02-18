// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateMaterialExpressionImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FCreateMaterialExpressionImplTool::FCreateMaterialExpressionImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FCreateMaterialExpressionImplTool::GetName() const
{
	return TEXT("create_material_expression");
}

FString FCreateMaterialExpressionImplTool::GetDescription() const
{
	return TEXT("Create a material expression node in a material");
}

TSharedPtr<FJsonObject> FCreateMaterialExpressionImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	TSharedPtr<FJsonObject> ExpressionClassProp = MakeShared<FJsonObject>();
	ExpressionClassProp->SetStringField(TEXT("type"), TEXT("string"));
	ExpressionClassProp->SetStringField(TEXT("description"), TEXT("Expression class name, e.g. Constant, Constant3Vector, ScalarParameter, VectorParameter, TextureSample, Add, Multiply"));
	Properties->SetObjectField(TEXT("expression_class"), ExpressionClassProp);

	TSharedPtr<FJsonObject> NodePosXProp = MakeShared<FJsonObject>();
	NodePosXProp->SetStringField(TEXT("type"), TEXT("number"));
	NodePosXProp->SetStringField(TEXT("description"), TEXT("X position of the node in the material editor"));
	NodePosXProp->SetNumberField(TEXT("default"), 0);
	Properties->SetObjectField(TEXT("node_pos_x"), NodePosXProp);

	TSharedPtr<FJsonObject> NodePosYProp = MakeShared<FJsonObject>();
	NodePosYProp->SetStringField(TEXT("type"), TEXT("number"));
	NodePosYProp->SetStringField(TEXT("description"), TEXT("Y position of the node in the material editor"));
	NodePosYProp->SetNumberField(TEXT("default"), 0);
	Properties->SetObjectField(TEXT("node_pos_y"), NodePosYProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("expression_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateMaterialExpressionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString MaterialPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: material_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ExpressionClass;
	if (!Arguments->TryGetStringField(TEXT("expression_class"), ExpressionClass))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: expression_class"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	double NodePosXValue = 0.0;
	Arguments->TryGetNumberField(TEXT("node_pos_x"), NodePosXValue);
	int32 NodePosX = static_cast<int32>(NodePosXValue);

	double NodePosYValue = 0.0;
	Arguments->TryGetNumberField(TEXT("node_pos_y"), NodePosYValue);
	int32 NodePosY = static_cast<int32>(NodePosYValue);

	FCreateMaterialExpressionResult ExprResult = MaterialModule.CreateMaterialExpression(MaterialPath, ExpressionClass, NodePosX, NodePosY);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ExprResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Material expression created successfully.\nExpressionName: %s\nExpressionClass: %s\nExpressionIndex: %d"),
			*ExprResult.ExpressionName,
			*ExprResult.ExpressionClass,
			ExprResult.ExpressionIndex);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create material expression: %s"), *ExprResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
