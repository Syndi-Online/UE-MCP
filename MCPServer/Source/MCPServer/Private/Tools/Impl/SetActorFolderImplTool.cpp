// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetActorFolderImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FSetActorFolderImplTool::FSetActorFolderImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FSetActorFolderImplTool::GetName() const
{
	return TEXT("set_actor_folder");
}

FString FSetActorFolderImplTool::GetDescription() const
{
	return TEXT("Set the folder path for an actor in the World Outliner hierarchy.");
}

TSharedPtr<FJsonObject> FSetActorFolderImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"), TEXT("Actor name, label, or path"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	TSharedPtr<FJsonObject> FolderProp = MakeShared<FJsonObject>();
	FolderProp->SetStringField(TEXT("type"), TEXT("string"));
	FolderProp->SetStringField(TEXT("description"),
		TEXT("Folder path in the World Outliner (e.g. 'Lighting/PointLights'). Use empty string to move to root."));
	Properties->SetObjectField(TEXT("folder_path"), FolderProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("folder_path")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetActorFolderImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier, FolderPath;
	if (!Arguments.IsValid() ||
		!Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier) ||
		!Arguments->TryGetStringField(TEXT("folder_path"), FolderPath))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameters: actor_identifier and folder_path"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FSetActorFolderResult FolderResult = ActorModule.SetActorFolder(ActorIdentifier, FolderPath);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (FolderResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Actor '%s' moved to folder '%s'"), *ActorIdentifier, *FolderPath));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set folder: %s"), *FolderResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
