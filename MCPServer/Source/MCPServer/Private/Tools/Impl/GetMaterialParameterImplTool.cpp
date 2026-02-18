// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetMaterialParameterImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FGetMaterialParameterImplTool::FGetMaterialParameterImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FGetMaterialParameterImplTool::GetName() const
{
	return TEXT("get_material_parameter");
}

FString FGetMaterialParameterImplTool::GetDescription() const
{
	return TEXT("Get a material parameter value (scalar, vector, texture, or static switch)");
}

TSharedPtr<FJsonObject> FGetMaterialParameterImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	TSharedPtr<FJsonObject> ParameterNameProp = MakeShared<FJsonObject>();
	ParameterNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ParameterNameProp->SetStringField(TEXT("description"), TEXT("Name of the parameter to get"));
	Properties->SetObjectField(TEXT("parameter_name"), ParameterNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("parameter_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetMaterialParameterImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString MaterialPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: material_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ParameterName;
	if (!Arguments->TryGetStringField(TEXT("parameter_name"), ParameterName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: parameter_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FGetMaterialParameterResult ParamResult = MaterialModule.GetMaterialParameter(MaterialPath, ParameterName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ParamResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Material parameter retrieved successfully.\nParameterName: %s\nParameterType: %s\nValue: %s"),
			*ParamResult.Parameter.ParameterName,
			*ParamResult.Parameter.ParameterType,
			*ParamResult.Parameter.Value);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get material parameter: %s"), *ParamResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
