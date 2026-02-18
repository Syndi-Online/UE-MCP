// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ConnectMaterialPropertyImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FConnectMaterialPropertyImplTool::FConnectMaterialPropertyImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FConnectMaterialPropertyImplTool::GetName() const
{
	return TEXT("connect_material_property");
}

FString FConnectMaterialPropertyImplTool::GetDescription() const
{
	return TEXT("Connect a material expression to a material output property");
}

TSharedPtr<FJsonObject> FConnectMaterialPropertyImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	TSharedPtr<FJsonObject> FromIndexProp = MakeShared<FJsonObject>();
	FromIndexProp->SetStringField(TEXT("type"), TEXT("number"));
	FromIndexProp->SetStringField(TEXT("description"), TEXT("Index of the source expression"));
	Properties->SetObjectField(TEXT("from_index"), FromIndexProp);

	TSharedPtr<FJsonObject> FromOutputProp = MakeShared<FJsonObject>();
	FromOutputProp->SetStringField(TEXT("type"), TEXT("string"));
	FromOutputProp->SetStringField(TEXT("description"), TEXT("Output pin name on the source expression"));
	FromOutputProp->SetStringField(TEXT("default"), TEXT(""));
	Properties->SetObjectField(TEXT("from_output"), FromOutputProp);

	TSharedPtr<FJsonObject> PropertyProp = MakeShared<FJsonObject>();
	PropertyProp->SetStringField(TEXT("type"), TEXT("string"));
	PropertyProp->SetStringField(TEXT("description"), TEXT("Material property: BaseColor, Metallic, Specular, Roughness, Normal, EmissiveColor, Opacity, OpacityMask, AmbientOcclusion, WorldPositionOffset, SubsurfaceColor, Refraction, Anisotropy, Tangent"));
	Properties->SetObjectField(TEXT("property"), PropertyProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("from_index")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("property")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FConnectMaterialPropertyImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	double FromIndexValue = 0.0;
	if (!Arguments->TryGetNumberField(TEXT("from_index"), FromIndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: from_index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString Property;
	if (!Arguments->TryGetStringField(TEXT("property"), Property))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: property"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 FromIndex = static_cast<int32>(FromIndexValue);

	FString FromOutput;
	Arguments->TryGetStringField(TEXT("from_output"), FromOutput);

	FConnectPropertyResult ConnectResult = MaterialModule.ConnectMaterialProperty(MaterialPath, FromIndex, FromOutput, Property);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ConnectResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Expression connected to material property successfully.\nPropertyName: %s"),
			*ConnectResult.PropertyName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to connect material property: %s"), *ConnectResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
