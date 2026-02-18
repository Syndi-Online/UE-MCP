// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchSetMaterialExpressionPropertiesImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FBatchSetMaterialExpressionPropertiesImplTool::FBatchSetMaterialExpressionPropertiesImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FBatchSetMaterialExpressionPropertiesImplTool::GetName() const
{
	return TEXT("batch_set_material_expression_properties");
}

FString FBatchSetMaterialExpressionPropertiesImplTool::GetDescription() const
{
	return TEXT("Set properties on multiple material expression nodes in one call. Up to 100 operations per batch.");
}

TSharedPtr<FJsonObject> FBatchSetMaterialExpressionPropertiesImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MatPathProp = MakeShared<FJsonObject>();
	MatPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MatPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MatPathProp);

	TSharedPtr<FJsonObject> OpsProp = MakeShared<FJsonObject>();
	OpsProp->SetStringField(TEXT("type"), TEXT("array"));
	OpsProp->SetStringField(TEXT("description"), TEXT("Array of operations. Each has expression_index, property_name, and property_value."));

	TSharedPtr<FJsonObject> ItemSchema = MakeShared<FJsonObject>();
	ItemSchema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> ItemProps = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IndexProp = MakeShared<FJsonObject>();
	IndexProp->SetStringField(TEXT("type"), TEXT("number"));
	ItemProps->SetObjectField(TEXT("expression_index"), IndexProp);

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	ItemProps->SetObjectField(TEXT("property_name"), NameProp);

	TSharedPtr<FJsonObject> ValueProp = MakeShared<FJsonObject>();
	ValueProp->SetStringField(TEXT("type"), TEXT("string"));
	ItemProps->SetObjectField(TEXT("property_value"), ValueProp);

	ItemSchema->SetObjectField(TEXT("properties"), ItemProps);
	OpsProp->SetObjectField(TEXT("items"), ItemSchema);
	Properties->SetObjectField(TEXT("operations"), OpsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("operations")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchSetMaterialExpressionPropertiesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	const TArray<TSharedPtr<FJsonValue>>* Operations = nullptr;
	if (!Arguments->TryGetArrayField(TEXT("operations"), Operations) || !Operations)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: operations (array)"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	if (Operations->Num() > 100)
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Too many operations. Maximum 100 per batch."));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	int32 Succeeded = 0;
	int32 Failed = 0;
	FString Errors;

	for (int32 i = 0; i < Operations->Num(); ++i)
	{
		const TSharedPtr<FJsonObject>* OpObj = nullptr;
		if (!(*Operations)[i]->TryGetObject(OpObj) || !OpObj || !(*OpObj).IsValid())
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Invalid operation object\n"), i);
			continue;
		}

		double ExpressionIndexValue = 0.0;
		FString PropertyName, PropertyValue;
		if (!(*OpObj)->TryGetNumberField(TEXT("expression_index"), ExpressionIndexValue) ||
			!(*OpObj)->TryGetStringField(TEXT("property_name"), PropertyName) ||
			!(*OpObj)->TryGetStringField(TEXT("property_value"), PropertyValue))
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Missing expression_index, property_name, or property_value\n"), i);
			continue;
		}

		int32 ExpressionIndex = static_cast<int32>(ExpressionIndexValue);
		FSetMaterialExpressionPropertyResult OpResult = MaterialModule.SetMaterialExpressionProperty(
			MaterialPath, ExpressionIndex, PropertyName, PropertyValue);

		if (OpResult.bSuccess)
		{
			Succeeded++;
		}
		else
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] %s\n"), i, *OpResult.ErrorMessage);
		}
	}

	FString ResponseText = FString::Printf(TEXT("Batch completed: %d succeeded, %d failed"), Succeeded, Failed);
	if (!Errors.IsEmpty())
	{
		ResponseText += TEXT("\n\nErrors:\n") + Errors;
	}

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), ResponseText);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	Result->SetBoolField(TEXT("isError"), Succeeded == 0 && Failed > 0);

	return Result;
}
