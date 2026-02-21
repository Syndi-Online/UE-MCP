// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetWidgetAnimationsImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FGetWidgetAnimationsImplTool::FGetWidgetAnimationsImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FGetWidgetAnimationsImplTool::GetName() const
{
	return TEXT("get_widget_animations");
}

FString FGetWidgetAnimationsImplTool::GetDescription() const
{
	return TEXT("List all widget animations in a Widget Blueprint");
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	// TODO: implement in commit 2
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("get_widget_animations is not yet implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
