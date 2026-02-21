// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RenameWidgetImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FRenameWidgetImplTool::FRenameWidgetImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FRenameWidgetImplTool::GetName() const
{
	return TEXT("rename_widget");
}

FString FRenameWidgetImplTool::GetDescription() const
{
	return TEXT("Rename a widget in a Widget Blueprint");
}

TSharedPtr<FJsonObject> FRenameWidgetImplTool::GetInputSchema() const
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
	WidgetNameProp->SetStringField(TEXT("description"), TEXT("Current name of the widget"));
	Properties->SetObjectField(TEXT("widget_name"), WidgetNameProp);

	TSharedPtr<FJsonObject> NewNameProp = MakeShared<FJsonObject>();
	NewNameProp->SetStringField(TEXT("type"), TEXT("string"));
	NewNameProp->SetStringField(TEXT("description"), TEXT("New name for the widget"));
	Properties->SetObjectField(TEXT("new_name"), NewNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("widget_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("new_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FRenameWidgetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("rename_widget is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
