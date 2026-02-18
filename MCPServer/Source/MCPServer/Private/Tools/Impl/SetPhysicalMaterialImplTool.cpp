// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetPhysicalMaterialImplTool.h"
#include "Modules/Interfaces/IPhysicsModule.h"
#include "Dom/JsonValue.h"

FSetPhysicalMaterialImplTool::FSetPhysicalMaterialImplTool(IPhysicsModule& InPhysicsModule)
	: PhysicsModule(InPhysicsModule)
{
}

FString FSetPhysicalMaterialImplTool::GetName() const
{
	return TEXT("set_physical_material");
}

FString FSetPhysicalMaterialImplTool::GetDescription() const
{
	return TEXT("Set the physical material override on an actor's primitive component");
}

TSharedPtr<FJsonObject> FSetPhysicalMaterialImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorNameProp = MakeShared<FJsonObject>();
	ActorNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ActorNameProp->SetStringField(TEXT("description"), TEXT("Name of the actor"));
	Properties->SetObjectField(TEXT("actor_name"), ActorNameProp);

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the physical material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetPhysicalMaterialImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorName;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_name"), ActorName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString MaterialPath;
	if (!Arguments->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: material_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FPhysicalMaterialResult MaterialResult = PhysicsModule.SetPhysicalMaterial(ActorName, MaterialPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (MaterialResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Physical material set successfully on actor: %s"),
			*ActorName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set physical material: %s"), *MaterialResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
