// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/CreateBlueprintFromActorImplTool.h"
#include "Modules/Interfaces/IBlueprintModule.h"
#include "Dom/JsonValue.h"

FCreateBlueprintFromActorImplTool::FCreateBlueprintFromActorImplTool(IBlueprintModule& InBlueprintModule)
	: BlueprintModule(InBlueprintModule)
{
}

FString FCreateBlueprintFromActorImplTool::GetName() const
{
	return TEXT("create_blueprint_from_actor");
}

FString FCreateBlueprintFromActorImplTool::GetDescription() const
{
	return TEXT("Create a Blueprint from an existing actor in the level");
}

TSharedPtr<FJsonObject> FCreateBlueprintFromActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> BlueprintPathProp = MakeShared<FJsonObject>();
	BlueprintPathProp->SetStringField(TEXT("type"), TEXT("string"));
	BlueprintPathProp->SetStringField(TEXT("description"), TEXT("Asset path for the new Blueprint"));
	Properties->SetObjectField(TEXT("blueprint_path"), BlueprintPathProp);

	TSharedPtr<FJsonObject> ActorIdentifierProp = MakeShared<FJsonObject>();
	ActorIdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	ActorIdentifierProp->SetStringField(TEXT("description"), TEXT("Name, label, or path of the source actor in the level"));
	Properties->SetObjectField(TEXT("actor_identifier"), ActorIdentifierProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("blueprint_path")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FCreateBlueprintFromActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
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

	FString ActorIdentifier;
	if (!Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_identifier"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FBlueprintFromActorResult CreateResult = BlueprintModule.CreateBlueprintFromActor(BlueprintPath, ActorIdentifier);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (CreateResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Blueprint created from actor successfully.\nBlueprintName: %s\nBlueprintPath: %s\nSourceActorName: %s"),
			*CreateResult.BlueprintName,
			*CreateResult.BlueprintPath,
			*CreateResult.SourceActorName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create Blueprint from actor: %s"), *CreateResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
