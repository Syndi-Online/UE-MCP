// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddEventDispatcherImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FAddEventDispatcherImplTool::FAddEventDispatcherImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FAddEventDispatcherImplTool::GetName() const
{
	return TEXT("add_event_dispatcher");
}

FString FAddEventDispatcherImplTool::GetDescription() const
{
	return TEXT("Add an Event Dispatcher (multicast delegate) to a Blueprint with optional parameters");
}

TSharedPtr<FJsonObject> FAddEventDispatcherImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> DispatcherNameProp = MakeShared<FJsonObject>();
	DispatcherNameProp->SetStringField(TEXT("type"), TEXT("string"));
	DispatcherNameProp->SetStringField(TEXT("description"), TEXT("Name of the Event Dispatcher to create"));
	Properties->SetObjectField(TEXT("dispatcher_name"), DispatcherNameProp);

	// Parameters array
	TSharedPtr<FJsonObject> ParamsProp = MakeShared<FJsonObject>();
	ParamsProp->SetStringField(TEXT("type"), TEXT("array"));
	ParamsProp->SetStringField(TEXT("description"), TEXT("Optional array of parameters for the dispatcher signature"));

	TSharedPtr<FJsonObject> ParamItemSchema = MakeShared<FJsonObject>();
	ParamItemSchema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> ParamItemProps = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> ParamNameProp = MakeShared<FJsonObject>();
	ParamNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ParamNameProp->SetStringField(TEXT("description"), TEXT("Parameter name"));
	ParamItemProps->SetObjectField(TEXT("name"), ParamNameProp);

	TSharedPtr<FJsonObject> ParamTypeProp = MakeShared<FJsonObject>();
	ParamTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	ParamTypeProp->SetStringField(TEXT("description"), TEXT("Type: Boolean, Integer, Int64, Float, String, Name, Text, Vector, Rotator, Transform, Byte, or class path"));
	ParamItemProps->SetObjectField(TEXT("type"), ParamTypeProp);

	ParamItemSchema->SetObjectField(TEXT("properties"), ParamItemProps);
	TArray<TSharedPtr<FJsonValue>> ParamItemRequired;
	ParamItemRequired.Add(MakeShared<FJsonValueString>(TEXT("name")));
	ParamItemRequired.Add(MakeShared<FJsonValueString>(TEXT("type")));
	ParamItemSchema->SetArrayField(TEXT("required"), ParamItemRequired);

	ParamsProp->SetObjectField(TEXT("items"), ParamItemSchema);
	Properties->SetObjectField(TEXT("parameters"), ParamsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("dispatcher_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddEventDispatcherImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString DispatcherName;
	if (!Arguments->TryGetStringField(TEXT("dispatcher_name"), DispatcherName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: dispatcher_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional parameters array
	TArray<FEventDispatcherParamInfo> Params;
	const TArray<TSharedPtr<FJsonValue>>* ParamsArray = nullptr;
	if (Arguments->TryGetArrayField(TEXT("parameters"), ParamsArray) && ParamsArray)
	{
		for (const TSharedPtr<FJsonValue>& ParamValue : *ParamsArray)
		{
			const TSharedPtr<FJsonObject>* ParamObj = nullptr;
			if (ParamValue.IsValid() && ParamValue->TryGetObject(ParamObj) && ParamObj && ParamObj->IsValid())
			{
				FEventDispatcherParamInfo Info;
				FString ParamName, ParamType;
				if ((*ParamObj)->TryGetStringField(TEXT("name"), ParamName) && (*ParamObj)->TryGetStringField(TEXT("type"), ParamType))
				{
					Info.ParamName = ParamName;
					Info.ParamType = ParamType;
					Params.Add(Info);
				}
			}
		}
	}

	FAddEventDispatcherResult DispatcherResult = BlueprintModule.AddEventDispatcher(BlueprintPath, DispatcherName, Params.Num() > 0 ? &Params : nullptr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DispatcherResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Event Dispatcher created successfully.\ndispatcher_name: %s\ngraph_name: %s"),
			*DispatcherResult.DispatcherName, *DispatcherResult.GraphName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add event dispatcher: %s"), *DispatcherResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
