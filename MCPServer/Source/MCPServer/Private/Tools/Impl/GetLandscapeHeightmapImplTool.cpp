// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetLandscapeHeightmapImplTool.h"
#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "Dom/JsonValue.h"

FGetLandscapeHeightmapImplTool::FGetLandscapeHeightmapImplTool(ILandscapeToolModule& InLandscapeModule)
	: LandscapeModule(InLandscapeModule)
{
}

FString FGetLandscapeHeightmapImplTool::GetName() const
{
	return TEXT("get_landscape_heightmap");
}

FString FGetLandscapeHeightmapImplTool::GetDescription() const
{
	return TEXT("Get landscape heightmap info: dimensions, scale, extent, and paint layer names");
}

TSharedPtr<FJsonObject> FGetLandscapeHeightmapImplTool::GetInputSchema() const
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

TSharedPtr<FJsonObject> FGetLandscapeHeightmapImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString LandscapeName;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("landscape_name"), LandscapeName);
	}

	FLandscapeHeightmapResult HmResult = LandscapeModule.GetLandscapeHeightmap(LandscapeName);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (HmResult.bSuccess)
	{
		TSharedPtr<FJsonObject> InfoJson = MakeShared<FJsonObject>();
		InfoJson->SetNumberField(TEXT("min_x"), HmResult.MinX);
		InfoJson->SetNumberField(TEXT("min_y"), HmResult.MinY);
		InfoJson->SetNumberField(TEXT("max_x"), HmResult.MaxX);
		InfoJson->SetNumberField(TEXT("max_y"), HmResult.MaxY);
		InfoJson->SetNumberField(TEXT("width"), HmResult.Width);
		InfoJson->SetNumberField(TEXT("height"), HmResult.Height);

		TSharedPtr<FJsonObject> ScaleJson = MakeShared<FJsonObject>();
		ScaleJson->SetNumberField(TEXT("x"), HmResult.Scale.X);
		ScaleJson->SetNumberField(TEXT("y"), HmResult.Scale.Y);
		ScaleJson->SetNumberField(TEXT("z"), HmResult.Scale.Z);
		InfoJson->SetObjectField(TEXT("scale"), ScaleJson);

		TArray<TSharedPtr<FJsonValue>> Layers;
		for (const FString& Name : HmResult.LayerNames)
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
			FString::Printf(TEXT("Failed to get heightmap: %s"), *HmResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
