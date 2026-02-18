// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetActorsInLevelImplTool.h"
#include "Modules/Interfaces/IActorModule.h"
#include "Dom/JsonValue.h"

FGetActorsInLevelImplTool::FGetActorsInLevelImplTool(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

FString FGetActorsInLevelImplTool::GetName() const
{
	return TEXT("get_actors_in_level");
}

FString FGetActorsInLevelImplTool::GetDescription() const
{
	return TEXT("Get a list of all actors in the current level. Optionally filter by class.");
}

TSharedPtr<FJsonObject> FGetActorsInLevelImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ClassFilterProp = MakeShared<FJsonObject>();
	ClassFilterProp->SetStringField(TEXT("type"), TEXT("string"));
	ClassFilterProp->SetStringField(TEXT("description"),
		TEXT("Optional class path to filter actors (e.g. '/Script/Engine.StaticMeshActor')"));
	Properties->SetObjectField(TEXT("class_filter"), ClassFilterProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FGetActorsInLevelImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ClassFilter;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("class_filter"), ClassFilter);
	}

	FGetActorsResult ActorsResult = ActorModule.GetActorsInLevel(ClassFilter);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ActorsResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Found %d actors in level:\n"), ActorsResult.Actors.Num());
		for (const FActorInfo& Info : ActorsResult.Actors)
		{
			ResponseText += FString::Printf(
				TEXT("- %s [%s] Class=%s Loc=(%.1f, %.1f, %.1f) Folder=%s\n"),
				*Info.ActorLabel, *Info.ActorName, *Info.ActorClass,
				Info.Location.X, Info.Location.Y, Info.Location.Z,
				*Info.FolderPath);
		}
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to get actors: %s"), *ActorsResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
