// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ExportLandscapeImplTool.h"
#include "Modules/Interfaces/ILandscapeToolModule.h"
#include "Dom/JsonValue.h"

FExportLandscapeImplTool::FExportLandscapeImplTool(ILandscapeToolModule& InLandscapeModule)
	: LandscapeModule(InLandscapeModule)
{
}

FString FExportLandscapeImplTool::GetName() const
{
	return TEXT("export_landscape");
}

FString FExportLandscapeImplTool::GetDescription() const
{
	return TEXT("Export landscape heightmap to a file (PNG format)");
}

TSharedPtr<FJsonObject> FExportLandscapeImplTool::GetInputSchema() const
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
	FileProp->SetStringField(TEXT("description"), TEXT("Output file path for the heightmap (e.g. D:/output/heightmap.png)"));
	Properties->SetObjectField(TEXT("output_file_path"), FileProp);

	Schema->SetObjectField(TEXT("properties"), Properties);
	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("output_file_path")));
	Schema->SetArrayField(TEXT("required"), Required);
	return Schema;
}

TSharedPtr<FJsonObject> FExportLandscapeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString OutputFilePath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("output_file_path"), OutputFilePath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: output_file_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString LandscapeName;
	Arguments->TryGetStringField(TEXT("landscape_name"), LandscapeName);

	FLandscapeExportResult ExportResult = LandscapeModule.ExportLandscape(LandscapeName, OutputFilePath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ExportResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Landscape heightmap exported to '%s'."), *ExportResult.FilePath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to export landscape: %s"), *ExportResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
