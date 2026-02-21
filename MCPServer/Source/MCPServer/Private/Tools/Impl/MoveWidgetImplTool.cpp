// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/MoveWidgetImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FMoveWidgetImplTool::FMoveWidgetImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FMoveWidgetImplTool::GetName() const
{
	return TEXT("move_widget");
}

FString FMoveWidgetImplTool::GetDescription() const
{
	return TEXT("Move a widget to a different parent panel in a Widget Blueprint");
}

TSharedPtr<FJsonObject> FMoveWidgetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> WidgetNameProp = MakeShared<FJsonObject>();
	WidgetNameProp->SetStringField(TEXT("type"), TEXT("string"));
	WidgetNameProp->SetStringField(TEXT("description"), TEXT("Name of the widget to move"));
	Properties->SetObjectField(TEXT("widget_name"), WidgetNameProp);

	TSharedPtr<FJsonObject> NewParentNameProp = MakeShared<FJsonObject>();
	NewParentNameProp->SetStringField(TEXT("type"), TEXT("string"));
	NewParentNameProp->SetStringField(TEXT("description"), TEXT("Name of the new parent panel widget"));
	Properties->SetObjectField(TEXT("new_parent_name"), NewParentNameProp);

	TSharedPtr<FJsonObject> InsertIndexProp = MakeShared<FJsonObject>();
	InsertIndexProp->SetStringField(TEXT("type"), TEXT("integer"));
	InsertIndexProp->SetStringField(TEXT("description"), TEXT("Index at which to insert in the new parent"));
	Properties->SetObjectField(TEXT("insert_index"), InsertIndexProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("widget_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("new_parent_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FMoveWidgetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString WidgetName;
	if (!Arguments->TryGetStringField(TEXT("widget_name"), WidgetName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: widget_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString NewParentName;
	if (!Arguments->TryGetStringField(TEXT("new_parent_name"), NewParentName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: new_parent_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 InsertIndex = -1;
	const int32* InsertIndexPtr = nullptr;
	double InsertIndexDouble;
	if (Arguments->TryGetNumberField(TEXT("insert_index"), InsertIndexDouble))
	{
		InsertIndex = static_cast<int32>(InsertIndexDouble);
		InsertIndexPtr = &InsertIndex;
	}

	FMoveWidgetResult MoveResult = UMGModule.MoveWidget(BlueprintPath, WidgetName, NewParentName, InsertIndexPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (MoveResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Widget moved successfully.\nWidgetName: %s\nNewParentName: %s"),
			*MoveResult.WidgetName,
			*MoveResult.NewParentName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to move widget: %s"), *MoveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
