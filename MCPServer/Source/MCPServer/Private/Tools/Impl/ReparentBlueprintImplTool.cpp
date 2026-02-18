// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ReparentBlueprintImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FReparentBlueprintImplTool::FReparentBlueprintImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FReparentBlueprintImplTool::GetName() const
{
	return TEXT("reparent_blueprint");
}

FString FReparentBlueprintImplTool::GetDescription() const
{
	return TEXT("Change the parent class of a Blueprint");
}

TSharedPtr<FJsonObject> FReparentBlueprintImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> NewParentClassProp = MakeShared<FJsonObject>();
	NewParentClassProp->SetStringField(TEXT("type"), TEXT("string"));
	NewParentClassProp->SetStringField(TEXT("description"), TEXT("Class path of the new parent class"));
	Properties->SetObjectField(TEXT("new_parent_class"), NewParentClassProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("new_parent_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FReparentBlueprintImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString NewParentClass;
	if (!Arguments->TryGetStringField(TEXT("new_parent_class"), NewParentClass))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: new_parent_class"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FBlueprintReparentResult ReparentResult = BlueprintModule.ReparentBlueprint(BlueprintPath, NewParentClass);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ReparentResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Blueprint reparented successfully.\nOldParentClass: %s\nNewParentClass: %s"),
			*ReparentResult.OldParentClass,
			*ReparentResult.NewParentClass);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to reparent Blueprint: %s"), *ReparentResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
