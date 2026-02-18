// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/BuildHlodWpImplTool.h"
#include "Modules/Interfaces/IWorldPartitionModule.h"
#include "Dom/JsonValue.h"

FBuildHlodWpImplTool::FBuildHlodWpImplTool(IWorldPartitionModule& InWorldPartitionModule)
	: WorldPartitionModule(InWorldPartitionModule)
{
}

FString FBuildHlodWpImplTool::GetName() const
{
	return TEXT("build_hlod_wp");
}

FString FBuildHlodWpImplTool::GetDescription() const
{
	return TEXT("Build HLOD for World Partition");
}

TSharedPtr<FJsonObject> FBuildHlodWpImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FBuildHlodWpImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FWorldPartitionResult WPResult = WorldPartitionModule.BuildHLOD();

	FString Message = WPResult.bSuccess ? WPResult.Message : WPResult.ErrorMessage;
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;
	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !WPResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
