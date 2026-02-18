// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DeleteMaterialExpressionImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FDeleteMaterialExpressionImplTool::FDeleteMaterialExpressionImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FDeleteMaterialExpressionImplTool::GetName() const
{
	return TEXT("delete_material_expression");
}

FString FDeleteMaterialExpressionImplTool::GetDescription() const
{
	return TEXT("Delete a material expression node from a material");
}

TSharedPtr<FJsonObject> FDeleteMaterialExpressionImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	TSharedPtr<FJsonObject> ExpressionIndexProp = MakeShared<FJsonObject>();
	ExpressionIndexProp->SetStringField(TEXT("type"), TEXT("number"));
	ExpressionIndexProp->SetStringField(TEXT("description"), TEXT("Index of the expression to delete"));
	Properties->SetObjectField(TEXT("expression_index"), ExpressionIndexProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("expression_index")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FDeleteMaterialExpressionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	double ExpressionIndexValue = 0.0;
	if (!Arguments->TryGetNumberField(TEXT("expression_index"), ExpressionIndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: expression_index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 ExpressionIndex = static_cast<int32>(ExpressionIndexValue);

	FDeleteMaterialExpressionResult DeleteResult = MaterialModule.DeleteMaterialExpression(MaterialPath, ExpressionIndex);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DeleteResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Material expression deleted successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to delete material expression: %s"), *DeleteResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
