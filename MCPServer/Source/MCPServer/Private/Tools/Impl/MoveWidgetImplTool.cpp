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
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("move_widget is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
