// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetSelectedContentBrowserImplTool.h"
#include "Modules/Interfaces/IUIModule.h"
#include "Dom/JsonValue.h"

FGetSelectedContentBrowserImplTool::FGetSelectedContentBrowserImplTool(IUIModule& InUIModule)
	: UIModule(InUIModule)
{
}

FString FGetSelectedContentBrowserImplTool::GetName() const
{
	return TEXT("get_selected_content_browser");
}

FString FGetSelectedContentBrowserImplTool::GetDescription() const
{
	return TEXT("Get current selection in Content Browser");
}

TSharedPtr<FJsonObject> FGetSelectedContentBrowserImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FGetSelectedContentBrowserImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FContentBrowserSelectionResult SelectionResult = UIModule.GetSelectedContentBrowser();

	bool bSuccess = SelectionResult.bSuccess;
	FString Message;

	if (bSuccess)
	{
		if (!SelectionResult.SelectedAssets.IsEmpty())
		{
			Message += TEXT("Selected Assets:\n");
			for (const FString& Asset : SelectionResult.SelectedAssets)
			{
				Message += FString::Printf(TEXT("  %s\n"), *Asset);
			}
		}
		if (!SelectionResult.SelectedFolders.IsEmpty())
		{
			Message += TEXT("Selected Folders:\n");
			for (const FString& Folder : SelectionResult.SelectedFolders)
			{
				Message += FString::Printf(TEXT("  %s\n"), *Folder);
			}
		}
		if (Message.IsEmpty())
		{
			Message = TEXT("No items selected in Content Browser");
		}
	}
	else
	{
		Message = FString::Printf(TEXT("Failed to get Content Browser selection: %s"), *SelectionResult.ErrorMessage);
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
