// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateAnimAssetImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FCreateAnimAssetImplTool::FCreateAnimAssetImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FCreateAnimAssetImplTool::GetName() const
{
	return TEXT("create_anim_asset");
}

FString FCreateAnimAssetImplTool::GetDescription() const
{
	return TEXT("Create a new animation asset (AnimSequence, BlendSpace, BlendSpace1D, or AnimMontage)");
}

TSharedPtr<FJsonObject> FCreateAnimAssetImplTool::GetInputSchema() const
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
	AssetNameProp->SetStringField(TEXT("description"), TEXT("Name for the new animation asset"));
	Properties->SetObjectField(TEXT("asset_name"), AssetNameProp);

	TSharedPtr<FJsonObject> PackagePathProp = MakeShared<FJsonObject>();
	PackagePathProp->SetStringField(TEXT("type"), TEXT("string"));
	PackagePathProp->SetStringField(TEXT("description"), TEXT("Package path where the asset will be created, e.g. /Game/Animations"));
	Properties->SetObjectField(TEXT("package_path"), PackagePathProp);

	TSharedPtr<FJsonObject> AssetTypeProp = MakeShared<FJsonObject>();
	AssetTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetTypeProp->SetStringField(TEXT("description"), TEXT("Type of animation asset: AnimSequence, BlendSpace, BlendSpace1D, AnimMontage"));
	Properties->SetObjectField(TEXT("asset_type"), AssetTypeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("skeleton_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_name")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("package_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateAnimAssetImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString SkeletonPath, AssetName, PackagePath, AssetType;
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
	if (!Arguments->TryGetStringField(TEXT("asset_type"), AssetType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: asset_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FCreateAnimAssetResult AnimResult = SkeletalMeshModule.CreateAnimAsset(SkeletonPath, AssetName, PackagePath, AssetType);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (AnimResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Animation asset '%s' created at '%s'."),
			*AnimResult.AssetName, *AnimResult.AssetPath);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create animation asset: %s"), *AnimResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
