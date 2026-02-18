// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SccSubmitImplTool.h"
#include "Modules/Interfaces/ISourceControlModule.h"
#include "Dom/JsonValue.h"

FSccSubmitImplTool::FSccSubmitImplTool(ISCCModule& InSCCModule)
	: SCCModule(InSCCModule)
{
}

FString FSccSubmitImplTool::GetName() const
{
	return TEXT("scc_submit");
}

FString FSccSubmitImplTool::GetDescription() const
{
	return TEXT("Submit checked out files with a description");
}

TSharedPtr<FJsonObject> FSccSubmitImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> FilesProp = MakeShared<FJsonObject>();
	FilesProp->SetStringField(TEXT("type"), TEXT("array"));
	TSharedPtr<FJsonObject> ItemsSchema = MakeShared<FJsonObject>();
	ItemsSchema->SetStringField(TEXT("type"), TEXT("string"));
	FilesProp->SetObjectField(TEXT("items"), ItemsSchema);
	FilesProp->SetStringField(TEXT("description"), TEXT("Array of file paths"));
	Properties->SetObjectField(TEXT("files"), FilesProp);

	TSharedPtr<FJsonObject> DescriptionProp = MakeShared<FJsonObject>();
	DescriptionProp->SetStringField(TEXT("type"), TEXT("string"));
	DescriptionProp->SetStringField(TEXT("description"), TEXT("Changelist description for the submit"));
	Properties->SetObjectField(TEXT("description"), DescriptionProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("files")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("description")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSccSubmitImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	const TArray<TSharedPtr<FJsonValue>>* FilesArray;
	if (!Arguments->TryGetArrayField(TEXT("files"), FilesArray))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: files"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	TArray<FString> Files;
	for (const auto& Val : *FilesArray)
	{
		FString FilePath;
		if (Val->TryGetString(FilePath))
		{
			Files.Add(FilePath);
		}
	}

	FString Description;
	if (!Arguments->TryGetStringField(TEXT("description"), Description))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: description"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSCCOperationResult OpResult = SCCModule.Submit(Files, Description);

	FString Message = OpResult.bSuccess ? OpResult.Message : OpResult.ErrorMessage;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !OpResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
