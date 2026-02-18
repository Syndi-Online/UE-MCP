// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CloseAssetEditorImplTool.h"
#include "Modules/Interfaces/IUIModule.h"
#include "Dom/JsonValue.h"

FCloseAssetEditorImplTool::FCloseAssetEditorImplTool(IUIModule& InUIModule)
	: UIModule(InUIModule)
{
}

FString FCloseAssetEditorImplTool::GetName() const
{
	return TEXT("close_asset_editor");
}

FString FCloseAssetEditorImplTool::GetDescription() const
{
	return TEXT("Close the editor for an asset");
}

TSharedPtr<FJsonObject> FCloseAssetEditorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> AssetPathProp = MakeShared<FJsonObject>();
	AssetPathProp->SetStringField(TEXT("type"), TEXT("string"));
	AssetPathProp->SetStringField(TEXT("description"), TEXT("Asset path to close editor for"));
	Properties->SetObjectField(TEXT("asset_path"), AssetPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("asset_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCloseAssetEditorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FAssetEditorResult EditorResult = UIModule.CloseAssetEditor(AssetPath);

	FString Message = EditorResult.bSuccess
		? FString::Printf(TEXT("Editor %s for: %s"), TEXT("closed"), *EditorResult.AssetPath)
		: EditorResult.ErrorMessage;

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));
	TextContent->SetStringField(TEXT("text"), Message);
	Result->SetBoolField(TEXT("isError"), !EditorResult.bSuccess);
	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);
	return Result;
}
