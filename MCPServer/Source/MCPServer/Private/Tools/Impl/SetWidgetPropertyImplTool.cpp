// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetWidgetPropertyImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FSetWidgetPropertyImplTool::FSetWidgetPropertyImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FSetWidgetPropertyImplTool::GetName() const
{
	return TEXT("set_widget_property");
}

FString FSetWidgetPropertyImplTool::GetDescription() const
{
	return TEXT("Set a property value on a widget in a Widget Blueprint");
}

TSharedPtr<FJsonObject> FSetWidgetPropertyImplTool::GetInputSchema() const
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
	WidgetNameProp->SetStringField(TEXT("description"), TEXT("Name of the target widget"));
	Properties->SetObjectField(TEXT("widget_name"), WidgetNameProp);

	TSharedPtr<FJsonObject> PropertyNameProp = MakeShared<FJsonObject>();
	PropertyNameProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyNameProp->SetStringField(TEXT("description"), TEXT("Property name (e.g. Text, ColorAndOpacity, Font, Visibility, IsEnabled, RenderOpacity, ToolTipText, bIsVariable)"));
	Properties->SetObjectField(TEXT("property_name"), PropertyNameProp);

	TSharedPtr<FJsonObject> PropertyValueProp = MakeShared<FJsonObject>();
	PropertyValueProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyValueProp->SetStringField(TEXT("description"), TEXT("Property value as string (parsed according to property type)"));
	Properties->SetObjectField(TEXT("property_value"), PropertyValueProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("widget_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property_value")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetWidgetPropertyImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("set_widget_property is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
