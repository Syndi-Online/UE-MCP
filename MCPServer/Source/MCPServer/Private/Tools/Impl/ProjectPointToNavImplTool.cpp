// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ProjectPointToNavImplTool.h"
#include "Modules/Interfaces/INavigationModule.h"
#include "Dom/JsonValue.h"

FProjectPointToNavImplTool::FProjectPointToNavImplTool(INavigationModule& InNavigationModule)
	: NavigationModule(InNavigationModule)
{
}

FString FProjectPointToNavImplTool::GetName() const
{
	return TEXT("project_point_to_nav");
}

FString FProjectPointToNavImplTool::GetDescription() const
{
	return TEXT("Project a world position onto the navigation mesh");
}

TSharedPtr<FJsonObject> FProjectPointToNavImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// Coordinate property template
	TSharedPtr<FJsonObject> CoordProp = MakeShared<FJsonObject>();
	CoordProp->SetStringField(TEXT("type"), TEXT("number"));

	// point (required)
	TSharedPtr<FJsonObject> PointProp = MakeShared<FJsonObject>();
	PointProp->SetStringField(TEXT("type"), TEXT("object"));
	PointProp->SetStringField(TEXT("description"), TEXT("World position to project"));

	TSharedPtr<FJsonObject> PointProperties = MakeShared<FJsonObject>();
	PointProperties->SetObjectField(TEXT("x"), CoordProp);
	PointProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	PointProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	PointProp->SetObjectField(TEXT("properties"), PointProperties);

	Properties->SetObjectField(TEXT("point"), PointProp);

	// extent (optional)
	TSharedPtr<FJsonObject> ExtentProp = MakeShared<FJsonObject>();
	ExtentProp->SetStringField(TEXT("type"), TEXT("object"));
	ExtentProp->SetStringField(TEXT("description"), TEXT("Search extent (defaults to 50,50,250)"));

	TSharedPtr<FJsonObject> ExtentProperties = MakeShared<FJsonObject>();
	ExtentProperties->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*CoordProp));
	ExtentProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*CoordProp));
	ExtentProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*CoordProp));
	ExtentProp->SetObjectField(TEXT("properties"), ExtentProperties);

	Properties->SetObjectField(TEXT("extent"), ExtentProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("point")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FProjectPointToNavImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	// Parse point
	FVector Point = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* PointObj;
	if (Arguments.IsValid() && Arguments->TryGetObjectField(TEXT("point"), PointObj))
	{
		(*PointObj)->TryGetNumberField(TEXT("x"), Point.X);
		(*PointObj)->TryGetNumberField(TEXT("y"), Point.Y);
		(*PointObj)->TryGetNumberField(TEXT("z"), Point.Z);
	}

	// Parse extent (default 50,50,250)
	FVector Extent(50.0, 50.0, 250.0);
	const TSharedPtr<FJsonObject>* ExtentObj;
	if (Arguments.IsValid() && Arguments->TryGetObjectField(TEXT("extent"), ExtentObj))
	{
		(*ExtentObj)->TryGetNumberField(TEXT("x"), Extent.X);
		(*ExtentObj)->TryGetNumberField(TEXT("y"), Extent.Y);
		(*ExtentObj)->TryGetNumberField(TEXT("z"), Extent.Z);
	}

	FNavProjectPointResult ProjectResult = NavigationModule.ProjectPointToNav(Point, Extent);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ProjectResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Projected location: (X=%.2f, Y=%.2f, Z=%.2f)"),
				ProjectResult.ProjectedLocation.X,
				ProjectResult.ProjectedLocation.Y,
				ProjectResult.ProjectedLocation.Z));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to project point: %s"), *ProjectResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
