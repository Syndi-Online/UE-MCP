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

	FString Search;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("search"), Search) || Search.IsEmpty())
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: search"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ClassName;
	const FString* ClassNamePtr = nullptr;
	if (Arguments->TryGetStringField(TEXT("class_name"), ClassName) && !ClassName.IsEmpty())
	{
		ClassNamePtr = &ClassName;
	}

	int32 Limit = 10;
	double LimitD = 0;
	if (Arguments->TryGetNumberField(TEXT("limit"), LimitD) && LimitD > 0)
	{
		Limit = static_cast<int32>(LimitD);
	}

	bool bBlueprintCallableOnly = true;
	Arguments->TryGetBoolField(TEXT("blueprint_callable_only"), bBlueprintCallableOnly);

	FFindFunctionResult FindResult = BlueprintModule.FindFunction(Search, ClassNamePtr, Limit, bBlueprintCallableOnly);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (FindResult.bSuccess)
	{
		if (FindResult.Functions.Num() == 0)
		{
			TextContent->SetStringField(TEXT("text"),
				FString::Printf(TEXT("No functions found matching \"%s\""), *Search));
			Result->SetBoolField(TEXT("isError"), false);
		}
		else
		{
			FString ResponseText = FString::Printf(TEXT("Found %d functions matching \"%s\":\n"),
				FindResult.Functions.Num(), *Search);
			for (const FFindFunctionInfo& Info : FindResult.Functions)
			{
				ResponseText += FString::Printf(TEXT("- %s (%s)"),
					*Info.FunctionName, *Info.ClassName);
				if (!Info.DisplayName.IsEmpty())
				{
					ResponseText += FString::Printf(TEXT(" [%s]"), *Info.DisplayName);
				}
				if (Info.Params.Num() > 0)
				{
					ResponseText += TEXT(" params: ");
					for (int32 i = 0; i < Info.Params.Num(); ++i)
					{
						if (i > 0) ResponseText += TEXT(", ");
						ResponseText += Info.Params[i].ParamName;
					}
				}
				ResponseText += TEXT("\n");
			}
			TextContent->SetStringField(TEXT("text"), ResponseText);
			Result->SetBoolField(TEXT("isError"), false);
		}
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("find_function failed: %s"), *FindResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
