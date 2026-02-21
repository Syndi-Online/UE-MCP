// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateWidgetBlueprintImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FCreateWidgetBlueprintImplTool::FCreateWidgetBlueprintImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FCreateWidgetBlueprintImplTool::GetName() const
{
	return TEXT("create_widget_blueprint");
}

FString FCreateWidgetBlueprintImplTool::GetDescription() const
{
	return TEXT("Create a new Widget Blueprint with an optional root widget type");
}

TSharedPtr<FJsonObject> FCreateWidgetBlueprintImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path for the new Widget Blueprint, e.g. \"/Game/UI/WBP_MainMenu\""));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> RootWidgetClassProp = MakeShared<FJsonObject>();
	RootWidgetClassProp->SetStringField(TEXT("type"), TEXT("string"));
	RootWidgetClassProp->SetStringField(TEXT("description"), TEXT("Root widget class (default: CanvasPanel). Options: CanvasPanel, VerticalBox, HorizontalBox, Overlay, GridPanel, ScrollBox"));
	Properties->SetObjectField(TEXT("root_widget_class"), RootWidgetClassProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateWidgetBlueprintImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString RootWidgetClass;
	const FString* RootWidgetClassPtr = nullptr;
	if (Arguments->TryGetStringField(TEXT("root_widget_class"), RootWidgetClass))
	{
		RootWidgetClassPtr = &RootWidgetClass;
	}

	FCreateWidgetBlueprintResult CreateResult = UMGModule.CreateWidgetBlueprint(BlueprintPath, RootWidgetClassPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CreateResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Widget Blueprint created successfully.\nBlueprintName: %s\nBlueprintPath: %s\nRootWidgetClass: %s"),
			*CreateResult.BlueprintName,
			*CreateResult.BlueprintPath,
			*CreateResult.RootWidgetClass);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create Widget Blueprint: %s"), *CreateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
