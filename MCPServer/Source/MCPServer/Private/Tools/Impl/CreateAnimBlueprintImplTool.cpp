// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateAnimBlueprintImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FCreateAnimBlueprintImplTool::FCreateAnimBlueprintImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FCreateAnimBlueprintImplTool::GetName() const
{
	return TEXT("create_anim_blueprint");
}

FString FCreateAnimBlueprintImplTool::GetDescription() const
{
	return TEXT("Create a new Animation Blueprint for a skeleton");
}

TSharedPtr<FJsonObject> FCreateAnimBlueprintImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> SkeletonPathProp = MakeShared<FJsonObject>();
	SkeletonPathProp->SetStringField(TEXT("type"), TEXT("string"));
	SkeletonPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the skeleton to use"));
	Properties->SetObjectField(TEXT("skeleton_path"), SkeletonPathProp);

	TSharedPtr<FJsonObject> AssetNameProp = MakeShared<FJsonObject>();
	AssetNameProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetNameProp->SetStringField(TEXT("description"), TEXT("Name for the new Anim Blueprint"));
	Properties->SetObjectField(TEXT("asset_name"), AssetNameProp);

	TSharedPtr<FJsonObject> PackagePathProp = MakeShared<FJsonObject>();
	PackagePathProp->SetStringField(TEXT("type"), TEXT("string"));
	PackagePathProp->SetStringField(TEXT("description"), TEXT("Package path where the asset will be created, e.g. /Game/Blueprints"));
	Properties->SetObjectField(TEXT("package_path"), PackagePathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("skeleton_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("package_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateAnimBlueprintImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString SkeletonPath, AssetName, PackagePath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("skeleton_path"), SkeletonPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: skeleton_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}
	if (!Arguments->TryGetStringField(TEXT("asset_name"), AssetName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: asset_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}
	if (!Arguments->TryGetStringField(TEXT("package_path"), PackagePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: package_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FCreateAnimBlueprintResult AnimBPResult = SkeletalMeshModule.CreateAnimBlueprint(SkeletonPath, AssetName, PackagePath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (AnimBPResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Anim Blueprint '%s' created at '%s'."),
			*AnimBPResult.AssetName, *AnimBPResult.AssetPath);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create Anim Blueprint: %s"), *AnimBPResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
