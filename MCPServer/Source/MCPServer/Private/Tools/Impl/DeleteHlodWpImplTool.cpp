// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DeleteHlodWpImplTool.h"
#include "Modules/Interfaces/IWorldPartitionModule.h"
#include "Dom/JsonValue.h"

FDeleteHlodWpImplTool::FDeleteHlodWpImplTool(IWorldPartitionModule& InWorldPartitionModule)
	: WorldPartitionModule(InWorldPartitionModule)
{
}

FString FDeleteHlodWpImplTool::GetName() const
{
	return TEXT("delete_hlod_wp");
}

FString FDeleteHlodWpImplTool::GetDescription() const
{
	return TEXT("Delete HLOD data for World Partition");
}

TSharedPtr<FJsonObject> FDeleteHlodWpImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FDeleteHlodWpImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	FWorldPartitionResult WPResult = WorldPartitionModule.DeleteHLOD();

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
