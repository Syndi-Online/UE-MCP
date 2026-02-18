// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetViewModeImplTool.h"
#include "Modules/Interfaces/IViewportModule.h"
#include "Dom/JsonValue.h"

FSetViewModeImplTool::FSetViewModeImplTool(IViewportModule& InViewportModule)
	: ViewportModule(InViewportModule)
{
}

FString FSetViewModeImplTool::GetName() const
{
	return TEXT("set_view_mode");
}

FString FSetViewModeImplTool::GetDescription() const
{
	return TEXT("Set the viewport rendering mode (Lit, Unlit, Wireframe, etc.).");
}

TSharedPtr<FJsonObject> FSetViewModeImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ModeProp = MakeShared<FJsonObject>();
	ModeProp->SetStringField(TEXT("type"), TEXT("string"));
	ModeProp->SetStringField(TEXT("description"),
		TEXT("View mode: Lit, Unlit, Wireframe, BrushWireframe, LightingOnly, LightComplexity, ShaderComplexity, DetailLighting, CollisionPawn, CollisionVisibility"));
	Properties->SetObjectField(TEXT("mode"), ModeProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("mode")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetViewModeImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString Mode;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("mode"), Mode))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: mode"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSimpleResult ViewResult = ViewportModule.SetViewMode(Mode);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ViewResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), ViewResult.Message);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set view mode: %s"), *ViewResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
