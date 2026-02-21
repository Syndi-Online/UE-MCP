// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ExportWidgetsImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FExportWidgetsImplTool::FExportWidgetsImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FExportWidgetsImplTool::GetName() const
{
	return TEXT("export_widgets");
}

FString FExportWidgetsImplTool::GetDescription() const
{
	return TEXT("Export widgets from a Widget Blueprint as serialized text");
}

TSharedPtr<FJsonObject> FExportWidgetsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> WidgetNamesProp = MakeShared<FJsonObject>();
	WidgetNamesProp->SetStringField(TEXT("type"), TEXT("array"));
	WidgetNamesProp->SetStringField(TEXT("description"), TEXT("Names of widgets to export (exports all if omitted)"));
	TSharedPtr<FJsonObject> ItemsObj = MakeShared<FJsonObject>();
	ItemsObj->SetStringField(TEXT("type"), TEXT("string"));
	WidgetNamesProp->SetObjectField(TEXT("items"), ItemsObj);
	Properties->SetObjectField(TEXT("widget_names"), WidgetNamesProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FExportWidgetsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	TArray<FString> WidgetNames;
	const TArray<TSharedPtr<FJsonValue>>* NamesArray;
	const TArray<FString>* WidgetNamesPtr = nullptr;
	if (Arguments->TryGetArrayField(TEXT("widget_names"), NamesArray))
	{
		for (const auto& Val : *NamesArray)
		{
			FString Name;
			if (Val->TryGetString(Name))
			{
				WidgetNames.Add(Name);
			}
		}
		if (WidgetNames.Num() > 0)
		{
			WidgetNamesPtr = &WidgetNames;
		}
	}

	FExportWidgetsResult ExportResult = UMGModule.ExportWidgets(BlueprintPath, WidgetNamesPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ExportResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Exported widget(s).\nExportedText: %s"), *ExportResult.ExportedText));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to export widgets: %s"), *ExportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
