// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchConnectMaterialExpressionsImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FBatchConnectMaterialExpressionsImplTool::FBatchConnectMaterialExpressionsImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FBatchConnectMaterialExpressionsImplTool::GetName() const
{
	return TEXT("batch_connect_material_expressions");
}

FString FBatchConnectMaterialExpressionsImplTool::GetDescription() const
{
	return TEXT("Connect multiple material expression nodes in one call. Up to 100 connections per batch.");
}

TSharedPtr<FJsonObject> FBatchConnectMaterialExpressionsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MatPathProp = MakeShared<FJsonObject>();
	MatPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MatPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MatPathProp);

	TSharedPtr<FJsonObject> ConnsProp = MakeShared<FJsonObject>();
	ConnsProp->SetStringField(TEXT("type"), TEXT("array"));
	ConnsProp->SetStringField(TEXT("description"), TEXT("Array of connections. Each has from_index, to_index, from_output, and to_input."));

	TSharedPtr<FJsonObject> ItemSchema = MakeShared<FJsonObject>();
	ItemSchema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> ItemProps = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> FromIndexProp = MakeShared<FJsonObject>();
	FromIndexProp->SetStringField(TEXT("type"), TEXT("number"));
	ItemProps->SetObjectField(TEXT("from_index"), FromIndexProp);

	TSharedPtr<FJsonObject> ToIndexProp = MakeShared<FJsonObject>();
	ToIndexProp->SetStringField(TEXT("type"), TEXT("number"));
	ItemProps->SetObjectField(TEXT("to_index"), ToIndexProp);

	TSharedPtr<FJsonObject> FromOutputProp = MakeShared<FJsonObject>();
	FromOutputProp->SetStringField(TEXT("type"), TEXT("string"));
	FromOutputProp->SetStringField(TEXT("default"), TEXT(""));
	ItemProps->SetObjectField(TEXT("from_output"), FromOutputProp);

	TSharedPtr<FJsonObject> ToInputProp = MakeShared<FJsonObject>();
	ToInputProp->SetStringField(TEXT("type"), TEXT("string"));
	ItemProps->SetObjectField(TEXT("to_input"), ToInputProp);

	ItemSchema->SetObjectField(TEXT("properties"), ItemProps);
	ConnsProp->SetObjectField(TEXT("items"), ItemSchema);
	Properties->SetObjectField(TEXT("connections"), ConnsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("connections")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchConnectMaterialExpressionsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	const TArray<TSharedPtr<FJsonValue>>* Connections = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("connections"), Connections) || !Connections)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: connections (array)"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (Connections->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Too many connections. Maximum 100 per batch."));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	FString Errors;

	for (int32 i = 0; i < Connections->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* ConnObj = nullptr;
		if (!(*Connections)[i]->TryGetObject(ConnObj) || !ConnObj || !(*ConnObj).IsValid())
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Invalid connection object\n"), i);
			continue;
		}

		double FromIndexValue = 0.0, ToIndexValue = 0.0;
		FString ToInput;
		if (!(*ConnObj)->TryGetNumberField(TEXT("from_index"), FromIndexValue) ||
			!(*ConnObj)->TryGetNumberField(TEXT("to_index"), ToIndexValue) ||
			!(*ConnObj)->TryGetStringField(TEXT("to_input"), ToInput))
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Missing from_index, to_index, or to_input\n"), i);
			continue;
		}

		FString FromOutput;
		(*ConnObj)->TryGetStringField(TEXT("from_output"), FromOutput);

		int32 FromIndex = static_cast<int32>(FromIndexValue);
		int32 ToIndex = static_cast<int32>(ToIndexValue);

		FConnectExpressionsResult OpResult = MaterialModule.ConnectMaterialExpressions(
			MaterialPath, FromIndex, FromOutput, ToIndex, ToInput);

		if (OpResult.bSuccess)
		{
			Succeeded++;
		}
		else
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] %s\n"), i, *OpResult.ErrorMessage);
		}
	}

	FString ResponseText = FString::Printf(TEXT("Batch completed: %d succeeded, %d failed"), Succeeded, Failed);
	if (!Errors.IsEmpty())
	{
		ResponseText += TEXT("\n\nErrors:\n") + Errors;
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), ResponseText);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), Succeeded == 0 && Failed > 0);

	return Result;
}
