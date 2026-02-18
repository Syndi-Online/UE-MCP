// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/OpenAssetEditorImplTool.h"
#include "Modules/Interfaces/IUIModule.h"
#include "Dom/JsonValue.h"

FOpenAssetEditorImplTool::FOpenAssetEditorImplTool(IUIModule& InUIModule)
	: UIModule(InUIModule)
{
}

FString FOpenAssetEditorImplTool::GetName() const
{
	return TEXT("open_asset_editor");
}

FString FOpenAssetEditorImplTool::GetDescription() const
{
	return TEXT("Open the editor for an asset");
}

TSharedPtr<FJsonObject> FOpenAssetEditorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path to open editor for"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FOpenAssetEditorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString AssetPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("asset_path"), AssetPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: asset_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FAssetEditorResult EditorResult = UIModule.OpenAssetEditor(AssetPath);

	FString Message = EditorResult.bSuccess
		? FString::Printf(TEXT("Editor %s for: %s"), TEXT("opened"), *EditorResult.AssetPath)
		: EditorResult.ErrorMessage;

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !EditorResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
