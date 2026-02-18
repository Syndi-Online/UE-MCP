// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetConfigValueImplTool.h"
#include "Modules/Interfaces/IProjectSettingsModule.h"
#include "Dom/JsonValue.h"

FSetConfigValueImplTool::FSetConfigValueImplTool(IProjectSettingsModule& InModule)
	: ProjectSettingsModule(InModule)
{
}

FString FSetConfigValueImplTool::GetName() const
{
	return TEXT("set_config_value");
}

FString FSetConfigValueImplTool::GetDescription() const
{
	return TEXT("Write a value to project configuration files");
}

TSharedPtr<FJsonObject> FSetConfigValueImplTool::GetInputSchema() const
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
	SectionProp->SetStringField(TEXT("description"), TEXT("Config section name"));
	Properties->SetObjectField(TEXT("section"), SectionProp);

	TSharedPtr<FJsonObject> KeyProp = MakeShared<FJsonObject>();
	KeyProp->SetStringField(TEXT("type"), TEXT("string"));
	KeyProp->SetStringField(TEXT("description"), TEXT("Config key name"));
	Properties->SetObjectField(TEXT("key"), KeyProp);

	TSharedPtr<FJsonObject> ValueProp = MakeShared<FJsonObject>();
	ValueProp->SetStringField(TEXT("type"), TEXT("string"));
	ValueProp->SetStringField(TEXT("description"), TEXT("Value to set"));
	Properties->SetObjectField(TEXT("value"), ValueProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("config")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("section")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("key")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("value")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetConfigValueImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString Config, Section, Key, Value;

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

	if (!Arguments->TryGetStringField(TEXT("value"), Value))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: value"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FConfigValueResult CfgResult = ProjectSettingsModule.SetConfigValue(Config, Section, Key, Value);

	FString Message;
	if (CfgResult.bSuccess)
	{
		Message = FString::Printf(TEXT("[%s] %s = %s"), *CfgResult.Section, *CfgResult.Key, *CfgResult.Value);
	}
	else
	{
		Message = FString::Printf(TEXT("Failed to set config value: %s"), *CfgResult.ErrorMessage);
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !CfgResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
