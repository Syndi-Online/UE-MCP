// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ApplyAnimCompressionImplTool.h"
#include "Modules/Interfaces/ISkeletalMeshModule.h"
#include "Dom/JsonValue.h"

FApplyAnimCompressionImplTool::FApplyAnimCompressionImplTool(ISkeletalMeshModule& InSkeletalMeshModule)
	: SkeletalMeshModule(InSkeletalMeshModule)
{
}

FString FApplyAnimCompressionImplTool::GetName() const
{
	return TEXT("apply_anim_compression");
}

FString FApplyAnimCompressionImplTool::GetDescription() const
{
	return TEXT("Apply bone compression settings to an animation sequence");
}

TSharedPtr<FJsonObject> FApplyAnimCompressionImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AnimPathProp = MakeShared<FJsonObject>();
	AnimPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AnimPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the animation sequence"));
	Properties->SetObjectField(TEXT("anim_sequence_path"), AnimPathProp);

	TSharedPtr<FJsonObject> CompressionPathProp = MakeShared<FJsonObject>();
	CompressionPathProp->SetStringField(TEXT("type"), TEXT("string"));
	CompressionPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the bone compression settings (empty to use default)"));
	Properties->SetObjectField(TEXT("compression_settings_path"), CompressionPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("anim_sequence_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FApplyAnimCompressionImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AnimSequencePath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("anim_sequence_path"), AnimSequencePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: anim_sequence_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString CompressionSettingsPath;
	Arguments->TryGetStringField(TEXT("compression_settings_path"), CompressionSettingsPath);

	FApplyAnimCompressionResult CompResult = SkeletalMeshModule.ApplyAnimCompression(AnimSequencePath, CompressionSettingsPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CompResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Compression applied to '%s'."), *AnimSequencePath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to apply compression: %s"), *CompResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
