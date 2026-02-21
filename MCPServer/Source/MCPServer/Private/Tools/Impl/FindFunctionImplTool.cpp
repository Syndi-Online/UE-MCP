// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/FindFunctionImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FFindFunctionImplTool::FFindFunctionImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FFindFunctionImplTool::GetName() const
{
	return TEXT("find_function");
}

FString FFindFunctionImplTool::GetDescription() const
{
	return TEXT("Search for Blueprint-callable functions by name substring. Use before add_graph_node to find the exact function name (K2_ prefix, etc.).");
}

TSharedPtr<FJsonObject> FFindFunctionImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> SearchProp = MakeShared<FJsonObject>();
	SearchProp->SetStringField(TEXT("type"), TEXT("string"));
	SearchProp->SetStringField(TEXT("description"), TEXT("Search substring (case-insensitive, matched against function name and display name)"));
	Properties->SetObjectField(TEXT("search"), SearchProp);

	TSharedPtr<FJsonObject> ClassNameProp = MakeShared<FJsonObject>();
	ClassNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ClassNameProp->SetStringField(TEXT("description"), TEXT("Optional: restrict search to this class (e.g. 'KismetSystemLibrary', 'Actor'). If omitted, searches common engine classes."));
	Properties->SetObjectField(TEXT("class_name"), ClassNameProp);

	TSharedPtr<FJsonObject> LimitProp = MakeShared<FJsonObject>();
	LimitProp->SetStringField(TEXT("type"), TEXT("integer"));
	LimitProp->SetStringField(TEXT("description"), TEXT("Maximum number of results to return (default: 10)"));
	Properties->SetObjectField(TEXT("limit"), LimitProp);

	TSharedPtr<FJsonObject> CallableProp = MakeShared<FJsonObject>();
	CallableProp->SetStringField(TEXT("type"), TEXT("boolean"));
	CallableProp->SetStringField(TEXT("description"), TEXT("Only return BlueprintCallable functions (default: true)"));
	Properties->SetObjectField(TEXT("blueprint_callable_only"), CallableProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("search")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FFindFunctionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), TEXT("find_function: not implemented"));
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), true);
	return Result;
}
