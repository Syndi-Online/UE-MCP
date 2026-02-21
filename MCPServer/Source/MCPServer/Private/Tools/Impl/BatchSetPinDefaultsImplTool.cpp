// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchSetPinDefaultsImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FBatchSetPinDefaultsImplTool::FBatchSetPinDefaultsImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FBatchSetPinDefaultsImplTool::GetName() const
{
	return TEXT("batch_set_pin_defaults");
}

FString FBatchSetPinDefaultsImplTool::GetDescription() const
{
	return TEXT("Set default values on multiple pins in a single call (up to 100 operations per call)");
}

TSharedPtr<FJsonObject> FBatchSetPinDefaultsImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> OpsProp = MakeShared<FJsonObject>();
	OpsProp->SetStringField(TEXT("type"), TEXT("array"));
	OpsProp->SetStringField(TEXT("description"), TEXT("Array of operations (max 100). Each: {node_id, pin_name, value}"));
	Properties->SetObjectField(TEXT("operations"), OpsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("operations")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchSetPinDefaultsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath, GraphName;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath) ||
		!Arguments->TryGetStringField(TEXT("graph_name"), GraphName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path, graph_name, operations"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	const TArray<TSharedPtr<FJsonValue>>* OpsArray = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("operations"), OpsArray) || !OpsArray || OpsArray->Num() == 0)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing or empty required parameter: operations"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (OpsArray->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Maximum 100 operations per call"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	TArray<FString> Errors;

	for (int32 i = 0; i < OpsArray->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* OpObj = nullptr;
		if (!(*OpsArray)[i].IsValid() || !(*OpsArray)[i]->TryGetObject(OpObj) || !OpObj || !(*OpObj).IsValid())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Invalid operation object"), i));
			continue;
		}

		FString NodeId, PinName, Value;
		(*OpObj)->TryGetStringField(TEXT("node_id"), NodeId);
		(*OpObj)->TryGetStringField(TEXT("pin_name"), PinName);
		(*OpObj)->TryGetStringField(TEXT("value"), Value);

		if (NodeId.IsEmpty() || PinName.IsEmpty())
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] Missing node_id or pin_name"), i));
			continue;
		}

		FSetPinDefaultValueResult PinResult = BlueprintModule.SetPinDefaultValue(BlueprintPath, GraphName, NodeId, PinName, Value);
		if (PinResult.bSuccess)
		{
			++Succeeded;
		}
		else
		{
			++Failed;
			Errors.Add(FString::Printf(TEXT("[%d] %s"), i, *PinResult.ErrorMessage));
		}
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	FString ResponseText = FString::Printf(TEXT("Batch set pin defaults: %d succeeded, %d failed"), Succeeded, Failed);
	if (Errors.Num() > 0)
	{
		ResponseText += TEXT("\nErrors:");
		for (const FString& Err : Errors)
		{
			ResponseText += FString::Printf(TEXT("\n- %s"), *Err);
		}
	}
	TextContent->SetStringField(TEXT("text"), ResponseText);

	Result->SetBoolField(TEXT("isError"), Succeeded == 0 && Failed > 0);

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
