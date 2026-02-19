// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchSetNodePositionsImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FBatchSetNodePositionsImplTool::FBatchSetNodePositionsImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FBatchSetNodePositionsImplTool::GetName() const
{
	return TEXT("batch_set_node_positions");
}

FString FBatchSetNodePositionsImplTool::GetDescription() const
{
	return TEXT("Move multiple nodes on a Blueprint graph in one call. Up to 100 operations per batch.");
}

TSharedPtr<FJsonObject> FBatchSetNodePositionsImplTool::GetInputSchema() const
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

	TSharedPtr<FJsonObject> OpsProp = MakeShared<FJsonObject>();
	OpsProp->SetStringField(TEXT("type"), TEXT("array"));
	OpsProp->SetStringField(TEXT("description"), TEXT("Array of operations. Each has node_id (string) and position {x, y} (numbers)."));

	TSharedPtr<FJsonObject> ItemSchema = MakeShared<FJsonObject>();
	ItemSchema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> ItemProps = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NodeIdProp = MakeShared<FJsonObject>();
	NodeIdProp->SetStringField(TEXT("type"), TEXT("string"));
	ItemProps->SetObjectField(TEXT("node_id"), NodeIdProp);

	TSharedPtr<FJsonObject> PositionProp = MakeShared<FJsonObject>();
	PositionProp->SetStringField(TEXT("type"), TEXT("object"));
	PositionProp->SetStringField(TEXT("description"), TEXT("New position {x, y}"));
	ItemProps->SetObjectField(TEXT("position"), PositionProp);

	ItemSchema->SetObjectField(TEXT("properties"), ItemProps);
	OpsProp->SetObjectField(TEXT("items"), ItemSchema);
	Properties->SetObjectField(TEXT("operations"), OpsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("graph_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("operations")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchSetNodePositionsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	const TArray<TSharedPtr<FJsonValue>>* Operations = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("operations"), Operations) || !Operations)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: operations (array)"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (Operations->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Too many operations. Maximum 100 per batch."));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	FString Errors;

	for (int32 i = 0; i < Operations->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* OpObj = nullptr;
		if (!(*Operations)[i]->TryGetObject(OpObj) || !OpObj || !(*OpObj).IsValid())
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Invalid operation object\n"), i);
			continue;
		}

		FString NodeId;
		if (!(*OpObj)->TryGetStringField(TEXT("node_id"), NodeId))
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Missing node_id\n"), i);
			continue;
		}

		const TSharedPtr<FJsonObject>* PositionObj;
		if (!(*OpObj)->TryGetObjectField(TEXT("position"), PositionObj))
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Missing position\n"), i);
			continue;
		}

		double PosXd = 0.0, PosYd = 0.0;
		(*PositionObj)->TryGetNumberField(TEXT("x"), PosXd);
		(*PositionObj)->TryGetNumberField(TEXT("y"), PosYd);
		int32 PosX = static_cast<int32>(PosXd);
		int32 PosY = static_cast<int32>(PosYd);

		FSetNodePositionResult OpResult = BlueprintModule.SetNodePosition(
			BlueprintPath, GraphName, NodeId, PosX, PosY);

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
