// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetLandscapeHeightmapImplTool.h"
#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "Dom/JsonValue.h"

FSetLandscapeHeightmapImplTool::FSetLandscapeHeightmapImplTool(ILandscapeToolModule& InLandscapeModule)
	: LandscapeModule(InLandscapeModule)
{
}

FString FSetLandscapeHeightmapImplTool::GetName() const
{
	return TEXT("set_landscape_heightmap");
}

FString FSetLandscapeHeightmapImplTool::GetDescription() const
{
	return TEXT("Import a heightmap file (PNG/RAW) into an existing landscape");
}

TSharedPtr<FJsonObject> FSetLandscapeHeightmapImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> NameProp = MakeShared<FJsonObject>();
	NameProp->SetStringField(TEXT("type"), TEXT("string"));
	NameProp->SetStringField(TEXT("description"), TEXT("Name of the landscape actor"));
	Properties->SetObjectField(TEXT("landscape_name"), NameProp);

	TSharedPtr<FJsonObject> FileProp = MakeShared<FJsonObject>();
	FileProp->SetStringField(TEXT("type"), TEXT("string"));
	FileProp->SetStringField(TEXT("description"), TEXT("File path to the heightmap image (PNG or RAW)"));
	Properties->SetObjectField(TEXT("file_path"), FileProp);

	Schema->SetObjectField(TEXT("properties"), Properties);
	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("file_path")));
	Schema->SetArrayField(TEXT("required"), Required);
	return Schema;
}

TSharedPtr<FJsonObject> FSetLandscapeHeightmapImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString FilePath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("file_path"), FilePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: file_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString LandscapeName;
	Arguments->TryGetStringField(TEXT("landscape_name"), LandscapeName);

	FLandscapeSetHeightmapResult SetResult = LandscapeModule.SetLandscapeHeightmap(LandscapeName, FilePath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (SetResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Heightmap imported from '%s' into landscape."), *FilePath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set heightmap: %s"), *SetResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
