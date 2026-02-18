// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SccDeleteImplTool.h"
#include "Modules/Interfaces/ISourceControlModule.h"
#include "Dom/JsonValue.h"

FSccDeleteImplTool::FSccDeleteImplTool(ISCCModule& InSCCModule)
	: SCCModule(InSCCModule)
{
}

FString FSccDeleteImplTool::GetName() const
{
	return TEXT("scc_delete");
}

FString FSccDeleteImplTool::GetDescription() const
{
	return TEXT("Mark files for deletion in source control");
}

TSharedPtr<FJsonObject> FSccDeleteImplTool::GetInputSchema() const
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

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("files")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSccDeleteImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FSCCOperationResult OpResult = SCCModule.Delete(Files);

	FString Message = OpResult.bSuccess ? OpResult.Message : OpResult.ErrorMessage;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !OpResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
