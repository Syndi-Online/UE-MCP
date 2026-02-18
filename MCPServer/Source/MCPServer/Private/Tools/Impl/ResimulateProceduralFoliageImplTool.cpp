// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ResimulateProceduralFoliageImplTool.h"
#include "Modules/Interfaces/IFoliageModule.h"
#include "Dom/JsonValue.h"

FResimulateProceduralFoliageImplTool::FResimulateProceduralFoliageImplTool(IFoliageModule& InFoliageModule)
	: FoliageModule(InFoliageModule)
{
}

FString FResimulateProceduralFoliageImplTool::GetName() const
{
	return TEXT("resimulate_procedural_foliage");
}

FString FResimulateProceduralFoliageImplTool::GetDescription() const
{
	return TEXT("Resimulate procedural foliage volumes in the current level");
}

TSharedPtr<FJsonObject> FResimulateProceduralFoliageImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> VolumeNameProp = MakeShared<FJsonObject>();
	VolumeNameProp->SetStringField(TEXT("type"), TEXT("string"));
	VolumeNameProp->SetStringField(TEXT("description"),
		TEXT("Name of specific procedural foliage volume (empty for all volumes)"));
	Properties->SetObjectField(TEXT("volume_name"), VolumeNameProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FResimulateProceduralFoliageImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString VolumeName;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("volume_name"), VolumeName);
	}

	FFoliageResimulateResult ResimulateResult = FoliageModule.ResimulateProceduralFoliage(VolumeName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ResimulateResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Resimulated %d procedural foliage volume(s)"), ResimulateResult.VolumesProcessed));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to resimulate procedural foliage: %s"), *ResimulateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
