// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddWidgetImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FAddWidgetImplTool::FAddWidgetImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FAddWidgetImplTool::GetName() const
{
	return TEXT("add_widget");
}

FString FAddWidgetImplTool::GetDescription() const
{
	return TEXT("Add a widget to a parent panel in a Widget Blueprint");
}

TSharedPtr<FJsonObject> FAddWidgetImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> WidgetClassProp = MakeShared<FJsonObject>();
	WidgetClassProp->SetStringField(TEXT("type"), TEXT("string"));
	WidgetClassProp->SetStringField(TEXT("description"), TEXT("Widget class to add (e.g. TextBlock, Button, Image, VerticalBox, HorizontalBox, CanvasPanel, Overlay, SizeBox, Border, ScrollBox, CheckBox, Slider, ProgressBar, EditableTextBox, ComboBoxString, Spacer, RichTextBlock)"));
	Properties->SetObjectField(TEXT("widget_class"), WidgetClassProp);

	TSharedPtr<FJsonObject> ParentNameProp = MakeShared<FJsonObject>();
	ParentNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ParentNameProp->SetStringField(TEXT("description"), TEXT("Name of the parent panel widget to add to"));
	Properties->SetObjectField(TEXT("parent_name"), ParentNameProp);

	TSharedPtr<FJsonObject> WidgetNameProp = MakeShared<FJsonObject>();
	WidgetNameProp->SetStringField(TEXT("type"), TEXT("string"));
	WidgetNameProp->SetStringField(TEXT("description"), TEXT("Custom name for the new widget (auto-generated if omitted)"));
	Properties->SetObjectField(TEXT("widget_name"), WidgetNameProp);

	TSharedPtr<FJsonObject> InsertIndexProp = MakeShared<FJsonObject>();
	InsertIndexProp->SetStringField(TEXT("type"), TEXT("integer"));
	InsertIndexProp->SetStringField(TEXT("description"), TEXT("Index at which to insert the child (-1 or omit for end)"));
	Properties->SetObjectField(TEXT("insert_index"), InsertIndexProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("widget_class")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("parent_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddWidgetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("add_widget is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
