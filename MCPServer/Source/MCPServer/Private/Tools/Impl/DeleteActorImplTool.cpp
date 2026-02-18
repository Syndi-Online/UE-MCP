// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/DeleteActorImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FDeleteActorImplTool::FDeleteActorImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FDeleteActorImplTool::GetName() const
{
	return TEXT("delete_actor");
}

FString FDeleteActorImplTool::GetDescription() const
{
	return TEXT("Delete an actor from the editor world by name, label, or path.");
}

TSharedPtr<FJsonObject> FDeleteActorImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> IdentifierProp = MakeShared<FJsonObject>();
	IdentifierProp->SetStringField(TEXT("type"), TEXT("string"));
	IdentifierProp->SetStringField(TEXT("description"),
		TEXT("Actor name, label, or path (e.g. \"StaticMeshActor_0\" or \"PersistentLevel.StaticMeshActor_0\")"));
	Properties->SetObjectField(TEXT("actor_identifier"), IdentifierProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_identifier")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FDeleteActorImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorIdentifier;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_identifier"), ActorIdentifier))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_identifier"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FActorDeleteResult DeleteResult = ActorModule.DeleteActor(ActorIdentifier);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (DeleteResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Actor '%s' deleted successfully."), *DeleteResult.ActorName));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to delete actor: %s"), *DeleteResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
