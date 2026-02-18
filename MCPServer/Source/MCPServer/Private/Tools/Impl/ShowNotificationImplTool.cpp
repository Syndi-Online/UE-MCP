// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ShowNotificationImplTool.h"
#include "Modules/Interfaces/IUIModule.h"
#include "Dom/JsonValue.h"

FShowNotificationImplTool::FShowNotificationImplTool(IUIModule& InUIModule)
	: UIModule(InUIModule)
{
}

FString FShowNotificationImplTool::GetName() const
{
	return TEXT("show_notification");
}

FString FShowNotificationImplTool::GetDescription() const
{
	return TEXT("Show a toast notification in the editor");
}

TSharedPtr<FJsonObject> FShowNotificationImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// message
	TSharedPtr<FJsonObject> MessageProp = MakeShared<FJsonObject>();
	MessageProp->SetStringField(TEXT("type"), TEXT("string"));
	MessageProp->SetStringField(TEXT("description"), TEXT("The notification message to display"));
	Properties->SetObjectField(TEXT("message"), MessageProp);

	// sub_text (optional)
	TSharedPtr<FJsonObject> SubTextProp = MakeShared<FJsonObject>();
	SubTextProp->SetStringField(TEXT("type"), TEXT("string"));
	SubTextProp->SetStringField(TEXT("description"), TEXT("Optional sub-text displayed below the main message"));
	Properties->SetObjectField(TEXT("sub_text"), SubTextProp);

	// duration (optional)
	TSharedPtr<FJsonObject> DurationProp = MakeShared<FJsonObject>();
	DurationProp->SetStringField(TEXT("type"), TEXT("number"));
	DurationProp->SetStringField(TEXT("description"), TEXT("Duration in seconds, default 3.0"));
	Properties->SetObjectField(TEXT("duration"), DurationProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("message")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FShowNotificationImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse message (required)
	FString Message;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("message"), Message))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: message"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse sub_text (optional)
	FString SubText;
	Arguments->TryGetStringField(TEXT("sub_text"), SubText);

	// Parse duration (optional)
	double Duration = 3.0;
	Arguments->TryGetNumberField(TEXT("duration"), Duration);

	// Call the module
	FNotificationResult NotificationResult = UIModule.ShowNotification(Message, SubText, Duration);

	bool bSuccess = NotificationResult.bSuccess;
	FString ResponseMessage;
	if (bSuccess)
	{
		ResponseMessage = TEXT("Notification shown successfully");
	}
	else
	{
		ResponseMessage = NotificationResult.ErrorMessage;
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), ResponseMessage);
	Result->SetBoolField(TEXT("isError"), !bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
