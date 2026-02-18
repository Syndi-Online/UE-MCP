// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetLandscapeWeightmapImplTool.h"
#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "Dom/JsonValue.h"

FGetLandscapeWeightmapImplTool::FGetLandscapeWeightmapImplTool(ILandscapeToolModule& InLandscapeModule)
	: LandscapeModule(InLandscapeModule)
{
}

FString FGetLandscapeWeightmapImplTool::GetName() const
{
	return TEXT("get_landscape_weightmap");
}

FString FGetLandscapeWeightmapImplTool::GetDescription() const
{
	return TEXT("Get landscape weight/paint layer names");
}

TSharedPtr<FJsonObject> FGetLandscapeWeightmapImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"), TEXT("Name of the landscape actor (empty for first landscape in level)"));
	Properties->SetObjectField(TEXT("landscape_name"), NameProp);
	Schema->SetObjectField(TEXT("properties"), Properties);
	return Schema;
}

TSharedPtr<FJsonObject> FGetLandscapeWeightmapImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LandscapeName;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("landscape_name"), LandscapeName);
	}

	FLandscapeWeightmapResult WmResult = LandscapeModule.GetLandscapeWeightmap(LandscapeName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (WmResult.bSuccess)
	{
		TSharedPtr<FJsonObject> InfoJson = MakeShared<FJsonObject>();
		TArray<TSharedPtr<FJsonValue>> Layers;
		for (const FString& Name : WmResult.LayerNames)
		{
			Layers.Add(MakeShared<FJsonValueString>(Name));
		}
		InfoJson->SetArrayField(TEXT("layers"), Layers);

		FString ResponseText;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ResponseText);
		FJsonSerializer::Serialize(InfoJson.ToSharedRef(), Writer);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get weightmap: %s"), *WmResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
