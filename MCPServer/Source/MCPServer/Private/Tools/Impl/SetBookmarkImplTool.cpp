// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetBookmarkImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FSetBookmarkImplTool::FSetBookmarkImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FSetBookmarkImplTool::GetName() const
{
	return TEXT("set_bookmark");
}

FString FSetBookmarkImplTool::GetDescription() const
{
	return TEXT("Save the current camera position as a bookmark.");
}

TSharedPtr<FJsonObject> FSetBookmarkImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FSetBookmarkImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FSimpleResult BookmarkResult = ViewportModule.SetBookmark(Index);

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
			FString::Printf(TEXT("Failed to set bookmark: %s"), *BookmarkResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
