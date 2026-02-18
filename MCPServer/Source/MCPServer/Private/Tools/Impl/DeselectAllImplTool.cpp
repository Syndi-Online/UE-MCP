// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DeselectAllImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FDeselectAllImplTool::FDeselectAllImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FDeselectAllImplTool::GetName() const
{
	return TEXT("deselect_all");
}

FString FDeselectAllImplTool::GetDescription() const
{
	return TEXT("Deselect all currently selected actors in the editor.");
}

TSharedPtr<FJsonObject> FDeselectAllImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();
	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FDeselectAllImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FActorSelectionResult DeselectResult = ActorModule.DeselectAll();

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DeselectResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"), TEXT("All actors deselected"));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to deselect: %s"), *DeselectResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
