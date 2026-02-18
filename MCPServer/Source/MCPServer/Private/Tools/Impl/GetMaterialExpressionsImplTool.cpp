// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetMaterialExpressionsImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FGetMaterialExpressionsImplTool::FGetMaterialExpressionsImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FGetMaterialExpressionsImplTool::GetName() const
{
	return TEXT("get_material_expressions");
}

FString FGetMaterialExpressionsImplTool::GetDescription() const
{
	return TEXT("Get all expression nodes in a material with their index, name, class, and editor position");
}

TSharedPtr<FJsonObject> FGetMaterialExpressionsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetMaterialExpressionsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FGetMaterialExpressionsResult ExprResult = MaterialModule.GetMaterialExpressions(MaterialPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ExprResult.bSuccess)
	{
		TArray<TSharedPtr<FJsonValue>> ExpressionsArray;
		for (const FMCPMaterialExpressionInfo& Info : ExprResult.Expressions)
		{
			TSharedPtr<FJsonObject> ExprObj = MakeShared<FJsonObject>();
			ExprObj->SetNumberField(TEXT("index"), Info.Index);
			ExprObj->SetStringField(TEXT("name"), Info.Name);
			ExprObj->SetStringField(TEXT("class"), Info.Class);

			TSharedPtr<FJsonObject> PosObj = MakeShared<FJsonObject>();
			PosObj->SetNumberField(TEXT("x"), Info.PosX);
			PosObj->SetNumberField(TEXT("y"), Info.PosY);
			ExprObj->SetObjectField(TEXT("position"), PosObj);

			ExpressionsArray.Add(MakeShared<FJsonValueObject>(ExprObj));
		}

		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(ExpressionsArray, Writer);

		FString ResponseText = FString::Printf(
			TEXT("Material has %d expressions.\n%s"),
			ExprResult.Expressions.Num(),
			*JsonString);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get material expressions: %s"), *ExprResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
