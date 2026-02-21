// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetWidgetTreeImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

namespace
{
	void FormatWidgetTreeRecursive(const TArray<FWidgetInfo>& Widgets, FString& OutText, int32 IndentLevel)
	{
		FString Indent;
		for (int32 i = 0; i < IndentLevel; ++i)
		{
			Indent += TEXT("  ");
		}

		for (const FWidgetInfo& Widget : Widgets)
		{
			OutText += FString::Printf(TEXT("%s- %s (%s)\n"), *Indent, *Widget.WidgetName, *Widget.WidgetClass);
			if (Widget.Children.Num() > 0)
			{
				FormatWidgetTreeRecursive(Widget.Children, OutText, IndentLevel + 1);
			}
		}
	}
}

FGetWidgetTreeImplTool::FGetWidgetTreeImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FGetWidgetTreeImplTool::GetName() const
{
	return TEXT("get_widget_tree");
}

FString FGetWidgetTreeImplTool::GetDescription() const
{
	return TEXT("Get the widget hierarchy tree of a Widget Blueprint");
}

TSharedPtr<FJsonObject> FGetWidgetTreeImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FGetWidgetTreeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FGetWidgetTreeResult TreeResult = UMGModule.GetWidgetTree(BlueprintPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (TreeResult.bSuccess)
	{
		// Extract blueprint name from path
		FString BlueprintName = FPaths::GetBaseFilename(BlueprintPath);

		FString TreeText = FString::Printf(
			TEXT("Widget tree for %s:\nRootWidget: %s (%s)\n"),
			*BlueprintName,
			*TreeResult.RootWidgetName,
			*TreeResult.RootWidgetClass);

		FormatWidgetTreeRecursive(TreeResult.Widgets, TreeText, 1);

		// Count total widgets recursively
		TFunction<int32(const TArray<FWidgetInfo>&)> CountWidgets = [&CountWidgets](const TArray<FWidgetInfo>& Widgets) -> int32
		{
			int32 Count = Widgets.Num();
			for (const FWidgetInfo& Widget : Widgets)
			{
				Count += CountWidgets(Widget.Children);
			}
			return Count;
		};
		int32 TotalWidgets = CountWidgets(TreeResult.Widgets) + 1; // +1 for root

		TreeText += FString::Printf(TEXT("TotalWidgets: %d"), TotalWidgets);

		TextContent->SetStringField(TEXT("text"), TreeText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get widget tree: %s"), *TreeResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
