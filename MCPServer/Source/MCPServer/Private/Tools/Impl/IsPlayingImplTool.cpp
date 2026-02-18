// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/IsPlayingImplTool.h"
#include "Modules/Interfaces/IPIEModule.h"
#include "Dom/JsonValue.h"

FIsPlayingImplTool::FIsPlayingImplTool(IPIEModule& InPIEModule)
	: PIEModule(InPIEModule)
{
}

FString FIsPlayingImplTool::GetName() const
{
	return TEXT("is_playing");
}

FString FIsPlayingImplTool::GetDescription() const
{
	return TEXT("Check if a Play In Editor session is active");
}

TSharedPtr<FJsonObject> FIsPlayingImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FIsPlayingImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FPIEStatusResult StatusResult = PIEModule.IsPlaying();

	FString Message = FString::Printf(TEXT("Playing: %s, Simulating: %s"),
		StatusResult.bIsPlaying ? TEXT("true") : TEXT("false"),
		StatusResult.bIsSimulating ? TEXT("true") : TEXT("false"));

	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);

	Result->SetBoolField(TEXT("isError"), false);

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
