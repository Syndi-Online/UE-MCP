// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BatchSetActorPropertiesImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FBatchSetActorPropertiesImplTool::FBatchSetActorPropertiesImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FBatchSetActorPropertiesImplTool::GetName() const
{
	return TEXT("batch_set_actor_properties");
}

FString FBatchSetActorPropertiesImplTool::GetDescription() const
{
	return TEXT("Set properties on multiple actors in one call. Up to 100 operations per batch.");
}

TSharedPtr<FJsonObject> FBatchSetActorPropertiesImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> OpsProp = MakeShared<FJsonObject>();
	OpsProp->SetStringField(TEXT("type"), TEXT("array"));
	OpsProp->SetStringField(TEXT("description"), TEXT("Array of operations. Each has actor_identifier, property_name, and property_value."));

	TSharedPtr<FJsonObject> ItemSchema = MakeShared<FJsonObject>();
	ItemSchema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> ItemProps = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorProp = MakeShared<FJsonObject>();
	ActorProp->SetStringField(TEXT("type"), TEXT("string"));
	ItemProps->SetObjectField(TEXT("actor_identifier"), ActorProp);

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
	Required.Add(MakeShared<FJsonValueString>(TEXT("operations")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FBatchSetActorPropertiesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	const TArray<TSharedPtr<FJsonValue>>* Operations = nullptr;
	if (!Arguments.IsValid() || !Arguments->TryGetArrayField(TEXT("operations"), Operations) || !Operations)
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

		FString ActorIdentifier, PropertyName, PropertyValue;
		if (!(*OpObj)->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier) ||
			!(*OpObj)->TryGetStringField(TEXT("property_name"), PropertyName) ||
			!(*OpObj)->TryGetStringField(TEXT("property_value"), PropertyValue))
		{
			Failed++;
			Errors += FString::Printf(TEXT("- [%d] Missing actor_identifier, property_name, or property_value\n"), i);
			continue;
		}

		FActorSetPropertyResult OpResult = ActorModule.SetActorProperty(ActorIdentifier, PropertyName, PropertyValue);
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
