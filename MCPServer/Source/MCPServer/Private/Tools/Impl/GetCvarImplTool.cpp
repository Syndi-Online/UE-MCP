// Copyright Epic Games, Inc. All Rights Reserved.
#include "Tools/Impl/GetCvarImplTool.h"
#include "Modules/Interfaces/IConsoleModule.h"
#include "Dom/JsonValue.h"

FGetCvarImplTool::FGetCvarImplTool(IConsoleModule& InConsoleModule)
	: ConsoleModule(InConsoleModule)
{
}

FString FGetCvarImplTool::GetName() const
{
	return TEXT("get_cvar");
}

FString FGetCvarImplTool::GetDescription() const
{
	return TEXT("Get the value of a console variable");
}

TSharedPtr<FJsonObject> FGetCvarImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"), TEXT("Name of the console variable"));
	Properties->SetObjectField(TEXT("name"), NameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetCvarImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FCVarResult CvarResult = ConsoleModule.GetCVar(Name);

	FString Message;
	if (CvarResult.bSuccess)
	{
		Message = FString::Printf(TEXT("%s = %s"), *CvarResult.Name, *CvarResult.Value);
	}
	else
	{
		Message = CvarResult.ErrorMessage;
	}

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !CvarResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
