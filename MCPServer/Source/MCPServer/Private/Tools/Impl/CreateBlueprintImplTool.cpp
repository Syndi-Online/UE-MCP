// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateBlueprintImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FCreateBlueprintImplTool::FCreateBlueprintImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FCreateBlueprintImplTool::GetName() const
{
	return TEXT("create_blueprint");
}

FString FCreateBlueprintImplTool::GetDescription() const
{
	return TEXT("Create a new Blueprint from a parent class");
}

TSharedPtr<FJsonObject> FCreateBlueprintImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path for the new Blueprint, e.g. \"/Game/Blueprints/MyBP\""));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> ParentClassProp = MakeShared<FJsonObject>();
	ParentClassProp->SetStringField(TEXT("type"), TEXT("string"));
	ParentClassProp->SetStringField(TEXT("description"), TEXT("Parent class path, e.g. \"/Script/Engine.Actor\""));
	Properties->SetObjectField(TEXT("parent_class"), ParentClassProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("parent_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateBlueprintImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString ParentClass;
	if (!Arguments->TryGetStringField(TEXT("parent_class"), ParentClass))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: parent_class"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FBlueprintCreateResult CreateResult = BlueprintModule.CreateBlueprint(BlueprintPath, ParentClass);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CreateResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Blueprint created successfully.\nBlueprintName: %s\nBlueprintPath: %s\nParentClass: %s"),
			*CreateResult.BlueprintName,
			*CreateResult.BlueprintPath,
			*CreateResult.ParentClass);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create Blueprint: %s"), *CreateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
