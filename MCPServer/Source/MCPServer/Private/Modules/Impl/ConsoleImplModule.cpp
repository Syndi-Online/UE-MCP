// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/ConsoleImplModule.h"
#include "Editor.h"
#include "Misc/StringOutputDevice.h"
#include "HAL/IConsoleManager.h"

FConsoleCommandResult FConsoleImplModule::ExecuteConsoleCommand(const FString& Command)
{
	FConsoleCommandResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	FStringOutputDevice OutputDevice;
	World->Exec(World, *Command, OutputDevice);

	Result.bSuccess = true;
	Result.Output = OutputDevice;
	if (Result.Output.IsEmpty())
	{
		Result.Output = TEXT("Command executed successfully (no output).");
	}
	return Result;
}

FCVarResult FConsoleImplModule::GetCVar(const FString& Name)
{
	FCVarResult Result;

	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*Name);
	if (!CVar)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Console variable not found: %s"), *Name);
		return Result;
	}

	Result.bSuccess = true;
	Result.Name = Name;
	Result.Value = CVar->GetString();
	return Result;
}

FCVarResult FConsoleImplModule::SetCVar(const FString& Name, const FString& Value)
{
	FCVarResult Result;

	IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*Name);
	if (!CVar)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Console variable not found: %s"), *Name);
		return Result;
	}

	CVar->Set(*Value, ECVF_SetByConsole);

	Result.bSuccess = true;
	Result.Name = Name;
	Result.Value = CVar->GetString();
	return Result;
}
