// Copyright Epic Games, Inc. All Rights Reserved.
#include "Tools/Impl/ExecuteConsoleCommandImplTool.h"
#include "Modules/Interfaces/IConsoleModule.h"
#include "Dom/JsonValue.h"

FExecuteConsoleCommandImplTool::FExecuteConsoleCommandImplTool(IConsoleModule& InConsoleModule)
	: ConsoleModule(InConsoleModule)
{
}

FString FExecuteConsoleCommandImplTool::GetName() const
{
	return TEXT("execute_console_command");
}

FString FExecuteConsoleCommandImplTool::GetDescription() const
{
	return TEXT("Execute a console command in the editor");
}

TSharedPtr<FJsonObject> FExecuteConsoleCommandImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> CommandProp = MakeShared<FJsonObject>();
	CommandProp->SetStringField(TEXT("type"), TEXT("string"));
	CommandProp->SetStringField(TEXT("description"), TEXT("Console command to execute"));
	Properties->SetObjectField(TEXT("command"), CommandProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("command")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FExecuteConsoleCommandImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FString Command;
	if (!Arguments->TryGetStringField(TEXT("command"), Command))
	{
		TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> ContentArray;
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: command"));
		Result->SetBoolField(TEXT("isError"), true);
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		return Result;
	}

	FConsoleCommandResult CmdResult = ConsoleModule.ExecuteConsoleCommand(Command);

	FString Message = CmdResult.bSuccess ? CmdResult.Output : CmdResult.ErrorMessage;

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !CmdResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
