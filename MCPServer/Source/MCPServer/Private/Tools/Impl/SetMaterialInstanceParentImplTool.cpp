// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetMaterialInstanceParentImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FSetMaterialInstanceParentImplTool::FSetMaterialInstanceParentImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FSetMaterialInstanceParentImplTool::GetName() const
{
	return TEXT("set_material_instance_parent");
}

FString FSetMaterialInstanceParentImplTool::GetDescription() const
{
	return TEXT("Change the parent material of a Material Instance");
}

TSharedPtr<FJsonObject> FSetMaterialInstanceParentImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> InstancePathProp = MakeShared<FJsonObject>();
	InstancePathProp->SetStringField(TEXT("type"), TEXT("string"));
	InstancePathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Material Instance"));
	Properties->SetObjectField(TEXT("instance_path"), InstancePathProp);

	TSharedPtr<FJsonObject> ParentPathProp = MakeShared<FJsonObject>();
	ParentPathProp->SetStringField(TEXT("type"), TEXT("string"));
	ParentPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the new parent material"));
	Properties->SetObjectField(TEXT("parent_path"), ParentPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("instance_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("parent_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetMaterialInstanceParentImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString ParentPath;
	if (!Arguments->TryGetStringField(TEXT("parent_path"), ParentPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: parent_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetMaterialInstanceParentResult ParentResult = MaterialModule.SetMaterialInstanceParent(InstancePath, ParentPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ParentResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("Material instance parent set successfully."));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set material instance parent: %s"), *ParentResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
