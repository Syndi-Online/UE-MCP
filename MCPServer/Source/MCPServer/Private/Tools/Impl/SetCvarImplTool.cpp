// Copyright Epic Games, Inc. All Rights Reserved.
#include "Tools/Impl/SetCvarImplTool.h"
#include "Modules/Interfaces/IConsoleModule.h"
#include "Dom/JsonValue.h"

FSetCvarImplTool::FSetCvarImplTool(IConsoleModule& InConsoleModule)
	: ConsoleModule(InConsoleModule)
{
}

FString FSetCvarImplTool::GetName() const
{
	return TEXT("set_cvar");
}

FString FSetCvarImplTool::GetDescription() const
{
	return TEXT("Set the value of a console variable");
}

TSharedPtr<FJsonObject> FSetCvarImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"), TEXT("Name of the console variable"));
	Properties->SetObjectField(TEXT("name"), NameProp);

	TSharedPtr<FJsonObject> ValueProp = MakeShared<FJsonObject>();
	ValueProp->SetStringField(TEXT("type"), TEXT("string"));
	ValueProp->SetStringField(TEXT("description"), TEXT("New value for the console variable"));
	Properties->SetObjectField(TEXT("value"), ValueProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("value")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetCvarImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FString Name;
	if (!Arguments->TryGetStringField(TEXT("name"), Name))
	{
		TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> ContentArray;
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: name"));
		Result->SetBoolField(TEXT("isError"), true);
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		return Result;
	}

	FString Value;
	if (!Arguments->TryGetStringField(TEXT("value"), Value))
	{
		TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> ContentArray;
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: value"));
		Result->SetBoolField(TEXT("isError"), true);
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		return Result;
	}

	FCVarResult SetResult = ConsoleModule.SetCVar(Name, Value);

	FString Message;
	if (SetResult.bSuccess)
	{
		Message = FString::Printf(TEXT("%s set to %s"), *SetResult.Name, *SetResult.Value);
	}
	else
	{
		Message = SetResult.ErrorMessage;
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !SetResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
