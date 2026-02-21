// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ImportWidgetsImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FImportWidgetsImplTool::FImportWidgetsImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FImportWidgetsImplTool::GetName() const
{
	return TEXT("import_widgets");
}

FString FImportWidgetsImplTool::GetDescription() const
{
	return TEXT("Import widgets from serialized text into a Widget Blueprint");
}

TSharedPtr<FJsonObject> FImportWidgetsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> ExportedTextProp = MakeShared<FJsonObject>();
	ExportedTextProp->SetStringField(TEXT("type"), TEXT("string"));
	ExportedTextProp->SetStringField(TEXT("description"), TEXT("Serialized widget text from export_widgets"));
	Properties->SetObjectField(TEXT("exported_text"), ExportedTextProp);

	TSharedPtr<FJsonObject> ParentNameProp = MakeShared<FJsonObject>();
	ParentNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ParentNameProp->SetStringField(TEXT("description"), TEXT("Parent widget to import under (root if omitted)"));
	Properties->SetObjectField(TEXT("parent_name"), ParentNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("exported_text")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FImportWidgetsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath, ExportedText;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath) ||
		!Arguments->TryGetStringField(TEXT("exported_text"), ExportedText))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: blueprint_path and exported_text"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ParentName;
	const FString* ParentNamePtr = nullptr;
	if (Arguments->TryGetStringField(TEXT("parent_name"), ParentName))
	{
		ParentNamePtr = &ParentName;
	}

	FImportWidgetsResult ImportResult = UMGModule.ImportWidgets(BlueprintPath, ExportedText, ParentNamePtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ImportResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Imported %d widget(s)."), ImportResult.WidgetsImported));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to import widgets: %s"), *ImportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
