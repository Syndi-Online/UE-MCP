// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetConfigValueImplTool.h"
#include "Modules/Interfaces/IProjectSettingsModule.h"
#include "Dom/JsonValue.h"

FGetConfigValueImplTool::FGetConfigValueImplTool(IProjectSettingsModule& InModule)
	: ProjectSettingsModule(InModule)
{
}

FString FGetConfigValueImplTool::GetName() const
{
	return TEXT("get_config_value");
}

FString FGetConfigValueImplTool::GetDescription() const
{
	return TEXT("Read a value from project configuration files");
}

TSharedPtr<FJsonObject> FGetConfigValueImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ConfigProp = MakeShared<FJsonObject>();
	ConfigProp->SetStringField(TEXT("type"), TEXT("string"));
	ConfigProp->SetStringField(TEXT("description"), TEXT("Config file name: Engine, Game, Editor, Input, Scalability"));
	Properties->SetObjectField(TEXT("config"), ConfigProp);

	TSharedPtr<FJsonObject> SectionProp = MakeShared<FJsonObject>();
	SectionProp->SetStringField(TEXT("type"), TEXT("string"));
	SectionProp->SetStringField(TEXT("description"), TEXT("Config section name (e.g. /Script/Engine.RendererSettings)"));
	Properties->SetObjectField(TEXT("section"), SectionProp);

	TSharedPtr<FJsonObject> KeyProp = MakeShared<FJsonObject>();
	KeyProp->SetStringField(TEXT("type"), TEXT("string"));
	KeyProp->SetStringField(TEXT("description"), TEXT("Config key name"));
	Properties->SetObjectField(TEXT("key"), KeyProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("config")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("section")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("key")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetConfigValueImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString Config, Section, Key;

	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("config"), Config))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: config"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (!Arguments->TryGetStringField(TEXT("section"), Section))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: section"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (!Arguments->TryGetStringField(TEXT("key"), Key))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: key"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FConfigValueResult CfgResult = ProjectSettingsModule.GetConfigValue(Config, Section, Key);

	FString Message;
	if (CfgResult.bSuccess)
	{
		Message = FString::Printf(TEXT("[%s] %s = %s"), *CfgResult.Section, *CfgResult.Key, *CfgResult.Value);
	}
	else
	{
		Message = FString::Printf(TEXT("Failed to get config value: %s"), *CfgResult.ErrorMessage);
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !CfgResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
