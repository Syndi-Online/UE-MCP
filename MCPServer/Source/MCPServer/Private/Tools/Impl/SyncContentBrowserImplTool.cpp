// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SyncContentBrowserImplTool.h"
#include "Modules/Interfaces/IUIModule.h"
#include "Dom/JsonValue.h"

FSyncContentBrowserImplTool::FSyncContentBrowserImplTool(IUIModule& InUIModule)
	: UIModule(InUIModule)
{
}

FString FSyncContentBrowserImplTool::GetName() const
{
	return TEXT("sync_content_browser");
}

FString FSyncContentBrowserImplTool::GetDescription() const
{
	return TEXT("Navigate Content Browser to a path");
}

TSharedPtr<FJsonObject> FSyncContentBrowserImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> PathProp = MakeShared<FJsonObject>();
	PathProp->SetStringField(TEXT("type"), TEXT("string"));
	PathProp->SetStringField(TEXT("description"), TEXT("Asset or folder path to navigate to"));
	Properties->SetObjectField(TEXT("path"), PathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSyncContentBrowserImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString Path;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("path"), Path))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FNotificationResult NotificationResult = UIModule.SyncContentBrowser(Path);

	bool bSuccess = NotificationResult.bSuccess;
	FString Message;
	if (bSuccess)
	{
		Message = FString::Printf(TEXT("Content Browser navigated to: %s"), *Path);
	}
	else
	{
		Message = FString::Printf(TEXT("Failed to sync Content Browser: %s"), *NotificationResult.ErrorMessage);
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
