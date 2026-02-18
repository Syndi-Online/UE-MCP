// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelloWorldImplTool.h"
#include "Dom/JsonValue.h"

FString FHelloWorldImplTool::GetName() const
{
	return TEXT("hello_world");
}

FString FHelloWorldImplTool::GetDescription() const
{
	return TEXT("Returns a greeting from Unreal Engine. Use this to verify the MCP connection is working.");
}

TSharedPtr<FJsonObject> FHelloWorldImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProperty = MakeShared<FJsonObject>();
	NameProperty->SetStringField(TEXT("type"), TEXT("string"));
	NameProperty->SetStringField(TEXT("description"), TEXT("Name to greet"));
	Properties->SetObjectField(TEXT("name"), NameProperty);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FHelloWorldImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FString NameToGreet = TEXT("World");
	if (Arguments.IsValid() && Arguments->HasField(TEXT("name")))
	{
		NameToGreet = Arguments->GetStringField(TEXT("name"));
	}

	FString Greeting = FString::Printf(
		TEXT("Hello, %s! Greetings from Unreal Engine 5 MCP Server!"),
		*NameToGreet);

	// Build content array item
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Greeting);

	// Build result
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), false);

	return Result;
}
