// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CompileBlueprintImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FCompileBlueprintImplTool::FCompileBlueprintImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FCompileBlueprintImplTool::GetName() const
{
	return TEXT("compile_blueprint");
}

FString FCompileBlueprintImplTool::GetDescription() const
{
	return TEXT("Compile a Blueprint asset");
}

TSharedPtr<FJsonObject> FCompileBlueprintImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Blueprint to compile"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCompileBlueprintImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString BlueprintPath;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("blueprint_path"), BlueprintPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: blueprint_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FBlueprintCompileResult CompileResult = BlueprintModule.CompileBlueprint(BlueprintPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CompileResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Blueprint compiled successfully.\nNumErrors: %d\nNumWarnings: %d"),
			CompileResult.NumErrors,
			CompileResult.NumWarnings);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to compile Blueprint: %s"), *CompileResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
