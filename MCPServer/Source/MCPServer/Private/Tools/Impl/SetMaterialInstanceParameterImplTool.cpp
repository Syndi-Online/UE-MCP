// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetMaterialInstanceParameterImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FSetMaterialInstanceParameterImplTool::FSetMaterialInstanceParameterImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FSetMaterialInstanceParameterImplTool::GetName() const
{
	return TEXT("set_material_instance_parameter");
}

FString FSetMaterialInstanceParameterImplTool::GetDescription() const
{
	return TEXT("Set a parameter value on a Material Instance");
}

TSharedPtr<FJsonObject> FSetMaterialInstanceParameterImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> InstancePathProp = MakeShared<FJsonObject>();
	InstancePathProp->SetStringField(TEXT("type"), TEXT("string"));
	InstancePathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Material Instance"));
	Properties->SetObjectField(TEXT("instance_path"), InstancePathProp);

	TSharedPtr<FJsonObject> ParameterNameProp = MakeShared<FJsonObject>();
	ParameterNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ParameterNameProp->SetStringField(TEXT("description"), TEXT("Name of the parameter to set"));
	Properties->SetObjectField(TEXT("parameter_name"), ParameterNameProp);

	TSharedPtr<FJsonObject> ValueProp = MakeShared<FJsonObject>();
	ValueProp->SetStringField(TEXT("type"), TEXT("string"));
	ValueProp->SetStringField(TEXT("description"), TEXT("Parameter value as string"));
	Properties->SetObjectField(TEXT("value"), ValueProp);

	TSharedPtr<FJsonObject> ParameterTypeProp = MakeShared<FJsonObject>();
	ParameterTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	ParameterTypeProp->SetStringField(TEXT("description"), TEXT("Type: Scalar, Vector, Texture, StaticSwitch"));
	Properties->SetObjectField(TEXT("parameter_type"), ParameterTypeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("instance_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("parameter_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("value")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("parameter_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetMaterialInstanceParameterImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString InstancePath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("instance_path"), InstancePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: instance_path"));
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

	FString Value;
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

	FString ParameterType;
	if (!Arguments->TryGetStringField(TEXT("parameter_type"), ParameterType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: parameter_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetMaterialInstanceParamResult SetResult = MaterialModule.SetMaterialInstanceParameter(InstancePath, ParameterName, Value, ParameterType);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Material instance parameter set successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set material instance parameter: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
