// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ConnectMaterialExpressionsImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FConnectMaterialExpressionsImplTool::FConnectMaterialExpressionsImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FConnectMaterialExpressionsImplTool::GetName() const
{
	return TEXT("connect_material_expressions");
}

FString FConnectMaterialExpressionsImplTool::GetDescription() const
{
	return TEXT("Connect two material expression nodes");
}

TSharedPtr<FJsonObject> FConnectMaterialExpressionsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	TSharedPtr<FJsonObject> FromIndexProp = MakeShared<FJsonObject>();
	FromIndexProp->SetStringField(TEXT("type"), TEXT("number"));
	FromIndexProp->SetStringField(TEXT("description"), TEXT("Index of the source expression"));
	Properties->SetObjectField(TEXT("from_index"), FromIndexProp);

	TSharedPtr<FJsonObject> FromOutputProp = MakeShared<FJsonObject>();
	FromOutputProp->SetStringField(TEXT("type"), TEXT("string"));
	FromOutputProp->SetStringField(TEXT("description"), TEXT("Output pin name on the source expression"));
	FromOutputProp->SetStringField(TEXT("default"), TEXT(""));
	Properties->SetObjectField(TEXT("from_output"), FromOutputProp);

	TSharedPtr<FJsonObject> ToIndexProp = MakeShared<FJsonObject>();
	ToIndexProp->SetStringField(TEXT("type"), TEXT("number"));
	ToIndexProp->SetStringField(TEXT("description"), TEXT("Index of the destination expression"));
	Properties->SetObjectField(TEXT("to_index"), ToIndexProp);

	TSharedPtr<FJsonObject> ToInputProp = MakeShared<FJsonObject>();
	ToInputProp->SetStringField(TEXT("type"), TEXT("string"));
	ToInputProp->SetStringField(TEXT("description"), TEXT("Input pin name on the destination expression"));
	ToInputProp->SetStringField(TEXT("default"), TEXT(""));
	Properties->SetObjectField(TEXT("to_input"), ToInputProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("from_index")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("to_index")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FConnectMaterialExpressionsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	double FromIndexValue = 0.0;
	if (!Arguments->TryGetNumberField(TEXT("from_index"), FromIndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: from_index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	double ToIndexValue = 0.0;
	if (!Arguments->TryGetNumberField(TEXT("to_index"), ToIndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: to_index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 FromIndex = static_cast<int32>(FromIndexValue);
	int32 ToIndex = static_cast<int32>(ToIndexValue);

	FString FromOutput;
	Arguments->TryGetStringField(TEXT("from_output"), FromOutput);

	FString ToInput;
	Arguments->TryGetStringField(TEXT("to_input"), ToInput);

	FConnectExpressionsResult ConnectResult = MaterialModule.ConnectMaterialExpressions(MaterialPath, FromIndex, FromOutput, ToIndex, ToInput);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ConnectResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Material expressions connected successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to connect material expressions: %s"), *ConnectResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
