// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddBlueprintVariableImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FAddBlueprintVariableImplTool::FAddBlueprintVariableImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FAddBlueprintVariableImplTool::GetName() const
{
	return TEXT("add_blueprint_variable");
}

FString FAddBlueprintVariableImplTool::GetDescription() const
{
	return TEXT("Add a variable to a Blueprint");
}

TSharedPtr<FJsonObject> FAddBlueprintVariableImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> VariableNameProp = MakeShared<FJsonObject>();
	VariableNameProp->SetStringField(TEXT("type"), TEXT("string"));
	VariableNameProp->SetStringField(TEXT("description"), TEXT("Name of the variable to add"));
	Properties->SetObjectField(TEXT("variable_name"), VariableNameProp);

	TSharedPtr<FJsonObject> VariableTypeProp = MakeShared<FJsonObject>();
	VariableTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	VariableTypeProp->SetStringField(TEXT("description"), TEXT("Type: Boolean, Integer, Int64, Float, String, Name, Text, Vector, Rotator, Transform, Byte, or class path"));
	Properties->SetObjectField(TEXT("variable_type"), VariableTypeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("variable_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("variable_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddBlueprintVariableImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: blueprint_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString VariableName;
	if (!Arguments->TryGetStringField(TEXT("variable_name"), VariableName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: variable_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString VariableType;
	if (!Arguments->TryGetStringField(TEXT("variable_type"), VariableType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: variable_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FBlueprintVariableResult VarResult = BlueprintModule.AddBlueprintVariable(BlueprintPath, VariableName, VariableType);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (VarResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Variable added successfully.\nVariableName: %s"),
			*VarResult.VariableName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add variable: %s"), *VarResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
