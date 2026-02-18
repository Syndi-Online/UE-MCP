// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/TakeScreenshotImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FTakeScreenshotImplTool::FTakeScreenshotImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FTakeScreenshotImplTool::GetName() const
{
	return TEXT("take_screenshot");
}

FString FTakeScreenshotImplTool::GetDescription() const
{
	return TEXT("Take a screenshot of the viewport.");
}

TSharedPtr<FJsonObject> FTakeScreenshotImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> FilenameProp = MakeShared<FJsonObject>();
	FilenameProp->SetStringField(TEXT("type"), TEXT("string"));
	FilenameProp->SetStringField(TEXT("description"), TEXT("Output file path (optional, auto-generated if empty)"));
	Properties->SetObjectField(TEXT("filename"), FilenameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FTakeScreenshotImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString Filename;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("filename"), Filename);
	}

	FScreenshotResult ScreenshotResult = ViewportModule.TakeScreenshot(Filename);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ScreenshotResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Screenshot saved to: %s"), *ScreenshotResult.FilePath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to take screenshot: %s"), *ScreenshotResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
