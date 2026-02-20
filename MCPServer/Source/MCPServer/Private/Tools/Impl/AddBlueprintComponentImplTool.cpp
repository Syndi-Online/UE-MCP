// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/AddBlueprintComponentImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FAddBlueprintComponentImplTool::FAddBlueprintComponentImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FAddBlueprintComponentImplTool::GetName() const
{
	return TEXT("add_blueprint_component");
}

FString FAddBlueprintComponentImplTool::GetDescription() const
{
	return TEXT("Add a component to a Blueprint");
}

TSharedPtr<FJsonObject> FAddBlueprintComponentImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> ComponentClassProp = MakeShared<FJsonObject>();
	ComponentClassProp->SetStringField(TEXT("type"), TEXT("string"));
	ComponentClassProp->SetStringField(TEXT("description"), TEXT("Class of the component to add (e.g. StaticMeshComponent, PointLightComponent)"));
	Properties->SetObjectField(TEXT("component_class"), ComponentClassProp);

	TSharedPtr<FJsonObject> ComponentNameProp = MakeShared<FJsonObject>();
	ComponentNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ComponentNameProp->SetStringField(TEXT("description"), TEXT("Optional name for the component. Auto-generated if omitted."));
	Properties->SetObjectField(TEXT("component_name"), ComponentNameProp);

	TSharedPtr<FJsonObject> ParentComponentProp = MakeShared<FJsonObject>();
	ParentComponentProp->SetStringField(TEXT("type"), TEXT("string"));
	ParentComponentProp->SetStringField(TEXT("description"), TEXT("Optional name of the parent component to attach to"));
	Properties->SetObjectField(TEXT("parent_component"), ParentComponentProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("component_class")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FAddBlueprintComponentImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString ComponentClass;
	if (!Arguments->TryGetStringField(TEXT("component_class"), ComponentClass))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: component_class"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional component_name
	FString ComponentName;
	bool bHasComponentName = Arguments->TryGetStringField(TEXT("component_name"), ComponentName);
	const FString* ComponentNamePtr = bHasComponentName ? &ComponentName : nullptr;

	// Parse optional parent_component
	FString ParentComponent;
	bool bHasParentComponent = Arguments->TryGetStringField(TEXT("parent_component"), ParentComponent);
	const FString* ParentComponentPtr = bHasParentComponent ? &ParentComponent : nullptr;

	FAddBlueprintComponentResult CompResult = BlueprintModule.AddBlueprintComponent(
		BlueprintPath, ComponentClass, ComponentNamePtr, ParentComponentPtr);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CompResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Component '%s' (class: %s) added to blueprint, parent: %s"),
			*CompResult.ComponentName, *CompResult.ComponentClass, *CompResult.ParentComponent);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to add component: %s"), *CompResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
