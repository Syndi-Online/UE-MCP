// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/JumpToBookmarkImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FJumpToBookmarkImplTool::FJumpToBookmarkImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FJumpToBookmarkImplTool::GetName() const
{
	return TEXT("jump_to_bookmark");
}

FString FJumpToBookmarkImplTool::GetDescription() const
{
	return TEXT("Jump to a previously saved camera bookmark.");
}

TSharedPtr<FJsonObject> FJumpToBookmarkImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IndexProp = MakeShared<FJsonObject>();
	IndexProp->SetStringField(TEXT("type"), TEXT("number"));
	IndexProp->SetStringField(TEXT("description"), TEXT("Bookmark index (0-9)"));
	Properties->SetObjectField(TEXT("index"), IndexProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("index")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FJumpToBookmarkImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	double IndexValue = 0.0;
	if (!Arguments.IsValid() || !Arguments->TryGetNumberField(TEXT("index"), IndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Index = static_cast<int32>(IndexValue);

	FSimpleResult BookmarkResult = ViewportModule.JumpToBookmark(Index);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (BookmarkResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), BookmarkResult.Message);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to jump to bookmark: %s"), *BookmarkResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
