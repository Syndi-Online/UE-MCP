// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetMaterialStatisticsImplTool.h"
#include "Modules/Interfaces/IMaterialModule.h"
#include "Dom/JsonValue.h"

FGetMaterialStatisticsImplTool::FGetMaterialStatisticsImplTool(IMaterialModule& InMaterialModule)
	: MaterialModule(InMaterialModule)
{
}

FString FGetMaterialStatisticsImplTool::GetName() const
{
	return TEXT("get_material_statistics");
}

FString FGetMaterialStatisticsImplTool::GetDescription() const
{
	return TEXT("Get shader statistics for a material (instruction counts, samplers, etc.)");
}

TSharedPtr<FJsonObject> FGetMaterialStatisticsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MaterialPathProp = MakeShared<FJsonObject>();
	MaterialPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MaterialPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the material"));
	Properties->SetObjectField(TEXT("material_path"), MaterialPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("material_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetMaterialStatisticsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString MaterialPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("material_path"), MaterialPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: material_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FMaterialStatisticsResult StatsResult = MaterialModule.GetMaterialStatistics(MaterialPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (StatsResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Material statistics retrieved successfully.\n")
			TEXT("NumVertexShaderInstructions: %d\n")
			TEXT("NumPixelShaderInstructions: %d\n")
			TEXT("NumSamplers: %d\n")
			TEXT("NumVertexTextureSamples: %d\n")
			TEXT("NumPixelTextureSamples: %d\n")
			TEXT("NumVirtualTextureSamples: %d\n")
			TEXT("NumInterpolatorScalars: %d"),
			StatsResult.NumVertexShaderInstructions,
			StatsResult.NumPixelShaderInstructions,
			StatsResult.NumSamplers,
			StatsResult.NumVertexTextureSamples,
			StatsResult.NumPixelTextureSamples,
			StatsResult.NumVirtualTextureSamples,
			StatsResult.NumInterpolatorScalars);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get material statistics: %s"), *StatsResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
