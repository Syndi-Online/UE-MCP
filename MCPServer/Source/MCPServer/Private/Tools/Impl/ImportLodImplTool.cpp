// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ImportLodImplTool.h"
#include "Modules/Interfaces/IStaticMeshModule.h"
#include "Dom/JsonValue.h"

FImportLodImplTool::FImportLodImplTool(IStaticMeshModule& InStaticMeshModule)
	: StaticMeshModule(InStaticMeshModule)
{
}

FString FImportLodImplTool::GetName() const
{
	return TEXT("import_lod");
}

FString FImportLodImplTool::GetDescription() const
{
	return TEXT("Import a LOD level from an FBX file");
}

TSharedPtr<FJsonObject> FImportLodImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the static mesh"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	TSharedPtr<FJsonObject> LodIndexProp = MakeShared<FJsonObject>();
	LodIndexProp->SetStringField(TEXT("type"), TEXT("integer"));
	LodIndexProp->SetStringField(TEXT("description"), TEXT("LOD index to import into"));
	Properties->SetObjectField(TEXT("lod_index"), LodIndexProp);

	TSharedPtr<FJsonObject> SourceFileProp = MakeShared<FJsonObject>();
	SourceFileProp->SetStringField(TEXT("type"), TEXT("string"));
	SourceFileProp->SetStringField(TEXT("description"), TEXT("Absolute path to FBX file"));
	Properties->SetObjectField(TEXT("source_file"), SourceFileProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mesh_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("lod_index")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("source_file")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FImportLodImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString MeshPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("mesh_path"), MeshPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: mesh_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	double LodIndexValue = 0.0;
	if (!Arguments->TryGetNumberField(TEXT("lod_index"), LodIndexValue))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: lod_index"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}
	int32 LodIndex = static_cast<int32>(LodIndexValue);

	FString SourceFile;
	if (!Arguments->TryGetStringField(TEXT("source_file"), SourceFile))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: source_file"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FImportLodResult LodResult = StaticMeshModule.ImportLod(MeshPath, LodIndex, SourceFile);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (LodResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("LOD imported successfully for '%s'.\nLodIndex: %d"),
			*MeshPath, LodResult.LodIndex);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to import LOD: %s"), *LodResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
