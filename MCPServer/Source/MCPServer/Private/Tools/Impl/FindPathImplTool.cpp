// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/FindPathImplTool.h"
#include "Modules/Interfaces/INavigationModule.h"
#include "Dom/JsonValue.h"

FFindPathImplTool::FFindPathImplTool(INavigationModule& InNavigationModule)
	: NavigationModule(InNavigationModule)
{
}

FString FFindPathImplTool::GetName() const
{
	return TEXT("find_path");
}

FString FFindPathImplTool::GetDescription() const
{
	return TEXT("Find a navigation path between two world positions");
}

TSharedPtr<FJsonObject> FFindPathImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// Coordinate property template
	TSharedPtr<FJsonObject> CoordProp = MakeShared<FJsonObject>();
	CoordProp->SetStringField(TEXT("type"), TEXT("number"));

	// start
	TSharedPtr<FJsonObject> StartProp = MakeShared<FJsonObject>();
	StartProp->SetStringField(TEXT("type"), TEXT("object"));
	StartProp->SetStringField(TEXT("description"), TEXT("Start position"));

	TSharedPtr<FJsonObject> StartProperties = MakeShared<FJsonObject>();
	StartProperties->SetObjectField(TEXT("x"), CoordProp);
	StartProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	StartProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	StartProp->SetObjectField(TEXT("properties"), StartProperties);

	Properties->SetObjectField(TEXT("start"), StartProp);

	// end
	TSharedPtr<FJsonObject> EndProp = MakeShared<FJsonObject>();
	EndProp->SetStringField(TEXT("type"), TEXT("object"));
	EndProp->SetStringField(TEXT("description"), TEXT("End position"));

	TSharedPtr<FJsonObject> EndProperties = MakeShared<FJsonObject>();
	EndProperties->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*CoordProp));
	EndProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	EndProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	EndProp->SetObjectField(TEXT("properties"), EndProperties);

	Properties->SetObjectField(TEXT("end"), EndProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("start")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("end")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FFindPathImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse start
	FVector StartVec = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* StartObj;
	if (Arguments.IsValid() && Arguments->TryGetObjectField(TEXT("start"), StartObj))
	{
		(*StartObj)->TryGetNumberField(TEXT("x"), StartVec.X);
		(*StartObj)->TryGetNumberField(TEXT("y"), StartVec.Y);
		(*StartObj)->TryGetNumberField(TEXT("z"), StartVec.Z);
	}

	// Parse end
	FVector EndVec = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* EndObj;
	if (Arguments.IsValid() && Arguments->TryGetObjectField(TEXT("end"), EndObj))
	{
		(*EndObj)->TryGetNumberField(TEXT("x"), EndVec.X);
		(*EndObj)->TryGetNumberField(TEXT("y"), EndVec.Y);
		(*EndObj)->TryGetNumberField(TEXT("z"), EndVec.Z);
	}

	FNavFindPathResult PathResult = NavigationModule.FindPath(StartVec, EndVec);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (PathResult.bSuccess)
	{
		FString ResponseText = FString::Printf(TEXT("Path found (partial: %s, points: %d)\nWaypoints:\n"),
			PathResult.bPartialPath ? TEXT("true") : TEXT("false"),
			PathResult.PathPoints.Num());

		for (int32 i = 0; i < PathResult.PathPoints.Num(); ++i)
		{
			const FVector& Point = PathResult.PathPoints[i];
			ResponseText += FString::Printf(TEXT("  [%d] (X=%.2f, Y=%.2f, Z=%.2f)\n"), i, Point.X, Point.Y, Point.Z);
		}

		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to find path: %s"), *PathResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
