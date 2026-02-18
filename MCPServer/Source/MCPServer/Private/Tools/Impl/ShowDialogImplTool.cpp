// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ShowDialogImplTool.h"
#include "Modules/Interfaces/IUIModule.h"
#include "Dom/JsonValue.h"

FShowDialogImplTool::FShowDialogImplTool(IUIModule& InUIModule)
	: UIModule(InUIModule)
{
}

FString FShowDialogImplTool::GetName() const
{
	return TEXT("show_dialog");
}

FString FShowDialogImplTool::GetDescription() const
{
	return TEXT("Show a modal dialog in the editor");
}

TSharedPtr<FJsonObject> FShowDialogImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// title
	TSharedPtr<FJsonObject> TitleProp = MakeShared<FJsonObject>();
	TitleProp->SetStringField(TEXT("type"), TEXT("string"));
	TitleProp->SetStringField(TEXT("description"), TEXT("The dialog title"));
	Properties->SetObjectField(TEXT("title"), TitleProp);

	// message
	TSharedPtr<FJsonObject> MessageProp = MakeShared<FJsonObject>();
	MessageProp->SetStringField(TEXT("type"), TEXT("string"));
	MessageProp->SetStringField(TEXT("description"), TEXT("The dialog message"));
	Properties->SetObjectField(TEXT("message"), MessageProp);

	// type (optional)
	TSharedPtr<FJsonObject> TypeProp = MakeShared<FJsonObject>();
	TypeProp->SetStringField(TEXT("type"), TEXT("string"));
	TypeProp->SetStringField(TEXT("description"), TEXT("Dialog type: Ok, YesNo, OkCancel, YesNoCancel. Default: Ok"));
	Properties->SetObjectField(TEXT("type"), TypeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("title")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("message")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FShowDialogImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse title (required)
	FString Title;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("title"), Title))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: title"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse message (required)
	FString Message;
	if (!Arguments->TryGetStringField(TEXT("message"), Message))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: message"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse type (optional)
	FString Type = TEXT("Ok");
	Arguments->TryGetStringField(TEXT("type"), Type);

	// Call the module
	FDialogResult DialogResult = UIModule.ShowDialog(Title, Message, Type);

	bool bSuccess = DialogResult.bSuccess;
	FString ResponseMessage;
	if (bSuccess)
	{
		ResponseMessage = DialogResult.UserResponse;
	}
	else
	{
		ResponseMessage = DialogResult.ErrorMessage;
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), ResponseMessage);
	Result->SetBoolField(TEXT("isError"), !bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
