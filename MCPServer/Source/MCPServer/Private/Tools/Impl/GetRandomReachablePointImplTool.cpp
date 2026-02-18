// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/GetRandomReachablePointImplTool.h"
#include "Modules/Interfaces/INavigationModule.h"
#include "Dom/JsonValue.h"

FGetRandomReachablePointImplTool::FGetRandomReachablePointImplTool(INavigationModule& InNavigationModule)
	: NavigationModule(InNavigationModule)
{
}

FString FGetRandomReachablePointImplTool::GetName() const
{
	return TEXT("get_random_reachable_point");
}

FString FGetRandomReachablePointImplTool::GetDescription() const
{
	return TEXT("Get a random reachable point on the navigation mesh within a radius");
}

TSharedPtr<FJsonObject> FGetRandomReachablePointImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// Coordinate property template
	TSharedPtr<FJsonObject> CoordProp = MakeShared<FJsonObject>();
	CoordProp->SetStringField(TEXT("type"), TEXT("number"));

	// origin (required)
	TSharedPtr<FJsonObject> OriginProp = MakeShared<FJsonObject>();
	OriginProp->SetStringField(TEXT("type"), TEXT("object"));
	OriginProp->SetStringField(TEXT("description"), TEXT("Center of search area"));

	TSharedPtr<FJsonObject> OriginProperties = MakeShared<FJsonObject>();
	OriginProperties->SetObjectField(TEXT("x"), CoordProp);
	OriginProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	OriginProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	OriginProp->SetObjectField(TEXT("properties"), OriginProperties);

	Properties->SetObjectField(TEXT("origin"), OriginProp);

	// radius (required)
	TSharedPtr<FJsonObject> RadiusProp = MakeShared<FJsonObject>();
	RadiusProp->SetStringField(TEXT("type"), TEXT("number"));
	RadiusProp->SetStringField(TEXT("description"), TEXT("Search radius"));
	Properties->SetObjectField(TEXT("radius"), RadiusProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("origin")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("radius")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FGetRandomReachablePointImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse origin
	FVector Origin = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* OriginObj;
	if (Arguments.IsValid() && Arguments->TryGetObjectField(TEXT("origin"), OriginObj))
	{
		(*OriginObj)->TryGetNumberField(TEXT("x"), Origin.X);
		(*OriginObj)->TryGetNumberField(TEXT("y"), Origin.Y);
		(*OriginObj)->TryGetNumberField(TEXT("z"), Origin.Z);
	}

	// Parse radius
	double Radius = 0.0;
	if (Arguments.IsValid())
	{
		Arguments->TryGetNumberField(TEXT("radius"), Radius);
	}

	FNavRandomPointResult RandomResult = NavigationModule.GetRandomReachablePoint(Origin, static_cast<float>(Radius));

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (RandomResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Random reachable point: (X=%.2f, Y=%.2f, Z=%.2f)"),
				RandomResult.Location.X,
				RandomResult.Location.Y,
				RandomResult.Location.Z));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to find random reachable point: %s"), *RandomResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
