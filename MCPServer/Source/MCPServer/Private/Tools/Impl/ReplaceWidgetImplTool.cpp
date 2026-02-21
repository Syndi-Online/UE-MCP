// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ReplaceWidgetImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FReplaceWidgetImplTool::FReplaceWidgetImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FReplaceWidgetImplTool::GetName() const
{
	return TEXT("replace_widget");
}

FString FReplaceWidgetImplTool::GetDescription() const
{
	return TEXT("Replace a widget's type while preserving its children");
}

TSharedPtr<FJsonObject> FReplaceWidgetImplTool::GetInputSchema() const
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
	WidgetNameProp->SetStringField(TEXT("description"), TEXT("Name of the widget to replace"));
	Properties->SetObjectField(TEXT("widget_name"), WidgetNameProp);

	TSharedPtr<FJsonObject> NewWidgetClassProp = MakeShared<FJsonObject>();
	NewWidgetClassProp->SetStringField(TEXT("type"), TEXT("string"));
	NewWidgetClassProp->SetStringField(TEXT("description"), TEXT("New widget class to replace with"));
	Properties->SetObjectField(TEXT("new_widget_class"), NewWidgetClassProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("widget_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("new_widget_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FReplaceWidgetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("replace_widget is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
