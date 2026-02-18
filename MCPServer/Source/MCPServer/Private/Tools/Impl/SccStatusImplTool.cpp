// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SccStatusImplTool.h"
#include "Modules/Interfaces/ISourceControlModule.h"
#include "Dom/JsonValue.h"

FSccStatusImplTool::FSccStatusImplTool(ISCCModule& InSCCModule)
	: SCCModule(InSCCModule)
{
}

FString FSccStatusImplTool::GetName() const
{
	return TEXT("scc_status");
}

FString FSccStatusImplTool::GetDescription() const
{
	return TEXT("Get source control status of files");
}

TSharedPtr<FJsonObject> FSccStatusImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FSccStatusImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FSCCStatusResult StatusResult = SCCModule.Status(Files);

	FString Message;
	for (const auto& FileStatus : StatusResult.FileStatuses)
	{
		FString StatusStr;
		if (FileStatus.bIsCheckedOut) StatusStr += TEXT("CheckedOut ");
		if (FileStatus.bIsAdded) StatusStr += TEXT("Added ");
		if (FileStatus.bIsDeleted) StatusStr += TEXT("Deleted ");
		if (FileStatus.bIsModified) StatusStr += TEXT("Modified ");
		if (!FileStatus.bIsSourceControlled) StatusStr += TEXT("NotControlled ");
		if (FileStatus.bIsCheckedOutByOther) StatusStr += FString::Printf(TEXT("LockedBy(%s) "), *FileStatus.OtherUser);
		if (StatusStr.IsEmpty()) StatusStr = TEXT("UpToDate");
		Message += FString::Printf(TEXT("%s: %s\n"), *FileStatus.FilePath, *StatusStr.TrimEnd());
	}

	bool bSuccess = StatusResult.bSuccess;
	if (!bSuccess)
	{
		Message = StatusResult.ErrorMessage;
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
