// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ImportLandscapeImplTool.h"
#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "Dom/JsonValue.h"

FImportLandscapeImplTool::FImportLandscapeImplTool(ILandscapeToolModule& InLandscapeModule)
	: LandscapeModule(InLandscapeModule)
{
}

FString FImportLandscapeImplTool::GetName() const
{
	return TEXT("import_landscape");
}

FString FImportLandscapeImplTool::GetDescription() const
{
	return TEXT("Import a landscape from a heightmap file");
}

TSharedPtr<FJsonObject> FImportLandscapeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));
	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> FileProp = MakeShared<FJsonObject>();
	FileProp->SetStringField(TEXT("type"), TEXT("string"));
	FileProp->SetStringField(TEXT("description"), TEXT("File path to the heightmap image (PNG or RAW)"));
	Properties->SetObjectField(TEXT("heightmap_file_path"), FileProp);

	TSharedPtr<FJsonObject> LocProp = MakeShared<FJsonObject>();
	LocProp->SetStringField(TEXT("type"), TEXT("object"));
	LocProp->SetStringField(TEXT("description"), TEXT("World location {x, y, z} for the landscape"));
	Properties->SetObjectField(TEXT("location"), LocProp);

	TSharedPtr<FJsonObject> ScaleProp = MakeShared<FJsonObject>();
	ScaleProp->SetStringField(TEXT("type"), TEXT("object"));
	ScaleProp->SetStringField(TEXT("description"), TEXT("Scale {x, y, z} for the landscape (default: 100,100,100)"));
	Properties->SetObjectField(TEXT("scale"), ScaleProp);

	Schema->SetObjectField(TEXT("properties"), Properties);
	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("heightmap_file_path")));
	Schema->SetArrayField(TEXT("required"), Required);
	return Schema;
}

TSharedPtr<FJsonObject> FImportLandscapeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString HeightmapFilePath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("heightmap_file_path"), HeightmapFilePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: heightmap_file_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FVector Location = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* LocationObj;
	if (Arguments->TryGetObjectField(TEXT("location"), LocationObj))
	{
		Location.X = (*LocationObj)->GetNumberField(TEXT("x"));
		Location.Y = (*LocationObj)->GetNumberField(TEXT("y"));
		Location.Z = (*LocationObj)->GetNumberField(TEXT("z"));
	}

	FVector Scale = FVector(100.0, 100.0, 100.0);
	const TSharedPtr<FJsonObject>* ScaleObj;
	if (Arguments->TryGetObjectField(TEXT("scale"), ScaleObj))
	{
		Scale.X = (*ScaleObj)->GetNumberField(TEXT("x"));
		Scale.Y = (*ScaleObj)->GetNumberField(TEXT("y"));
		Scale.Z = (*ScaleObj)->GetNumberField(TEXT("z"));
	}

	FLandscapeImportResult ImportResult = LandscapeModule.ImportLandscape(HeightmapFilePath, Location, Scale);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ImportResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Landscape heightmap data loaded from '%s'."), *HeightmapFilePath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to import landscape: %s"), *ImportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
