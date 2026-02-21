// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetWidgetAnimationsImplTool.h"
#include "Modules/Interfaces/IUMGModule.h"
#include "Dom/JsonValue.h"

FGetWidgetAnimationsImplTool::FGetWidgetAnimationsImplTool(IUMGModule& InUMGModule)
	: UMGModule(InUMGModule)
{
}

FString FGetWidgetAnimationsImplTool::GetName() const
{
	return TEXT("get_widget_animations");
}

FString FGetWidgetAnimationsImplTool::GetDescription() const
{
	return TEXT("List all widget animations in a Widget Blueprint");
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path of the Widget Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetWidgetAnimationsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FGetWidgetAnimationsResult AnimResult = UMGModule.GetWidgetAnimations(BlueprintPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (AnimResult.bSuccess)
	{
		if (AnimResult.Animations.Num() == 0)
		{
			TextContent->SetStringField(TEXT("text"),
				FString::Printf(TEXT("No animations found in %s."), *BlueprintPath));
		}
		else
		{
			FString ResponseText = FString::Printf(TEXT("Animations in %s:\n"), *BlueprintPath);
			for (const FWidgetAnimationInfo& Anim : AnimResult.Animations)
			{
				ResponseText += FString::Printf(TEXT("  %s: %.2fs - %.2fs\n"),
					*Anim.AnimationName, Anim.StartTime, Anim.EndTime);
			}
			ResponseText += FString::Printf(TEXT("TotalAnimations: %d"), AnimResult.Animations.Num());
			TextContent->SetStringField(TEXT("text"), ResponseText);
		}
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get widget animations: %s"), *AnimResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
