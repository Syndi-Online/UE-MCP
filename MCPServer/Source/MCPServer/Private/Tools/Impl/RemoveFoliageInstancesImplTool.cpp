// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/RemoveFoliageInstancesImplTool.h"
#include "Modules/Interfaces/IFoliageModule.h"
#include "Dom/JsonValue.h"

FRemoveFoliageInstancesImplTool::FRemoveFoliageInstancesImplTool(IFoliageModule& InFoliageModule)
	: FoliageModule(InFoliageModule)
{
}

FString FRemoveFoliageInstancesImplTool::GetName() const
{
	return TEXT("remove_foliage_instances");
}

FString FRemoveFoliageInstancesImplTool::GetDescription() const
{
	return TEXT("Remove foliage instances within a sphere or all instances of a mesh type");
}

TSharedPtr<FJsonObject> FRemoveFoliageInstancesImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	// mesh_path
	TSharedPtr<FJsonObject> MeshPathProp = MakeShared<FJsonObject>();
	MeshPathProp->SetStringField(TEXT("type"), TEXT("string"));
	MeshPathProp->SetStringField(TEXT("description"),
		TEXT("Asset path of mesh type to remove (empty for all types)"));
	Properties->SetObjectField(TEXT("mesh_path"), MeshPathProp);

	// center
	TSharedPtr<FJsonObject> CenterProp = MakeShared<FJsonObject>();
	CenterProp->SetStringField(TEXT("type"), TEXT("object"));
	CenterProp->SetStringField(TEXT("description"), TEXT("Center of removal sphere"));
	TSharedPtr<FJsonObject> CenterProperties = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));
	CenterProperties->SetObjectField(TEXT("x"), NumProp);
	CenterProperties->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	CenterProperties->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	CenterProp->SetObjectField(TEXT("properties"), CenterProperties);
	Properties->SetObjectField(TEXT("center"), CenterProp);

	// radius
	TSharedPtr<FJsonObject> RadiusProp = MakeShared<FJsonObject>();
	RadiusProp->SetStringField(TEXT("type"), TEXT("number"));
	RadiusProp->SetStringField(TEXT("description"),
		TEXT("Radius of removal sphere (0 to remove all instances of the mesh type)"));
	Properties->SetObjectField(TEXT("radius"), RadiusProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	return Schema;
}

TSharedPtr<FJsonObject> FRemoveFoliageInstancesImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString MeshPath;
	if (Arguments.IsValid())
	{
		Arguments->TryGetStringField(TEXT("mesh_path"), MeshPath);
	}

	FVector Center = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* CenterObj;
	if (Arguments.IsValid() && Arguments->TryGetObjectField(TEXT("center"), CenterObj))
	{
		(*CenterObj)->TryGetNumberField(TEXT("x"), Center.X);
		(*CenterObj)->TryGetNumberField(TEXT("y"), Center.Y);
		(*CenterObj)->TryGetNumberField(TEXT("z"), Center.Z);
	}

	double Radius = 0.0;
	if (Arguments.IsValid())
	{
		Arguments->TryGetNumberField(TEXT("radius"), Radius);
	}

	FFoliageRemoveInstancesResult RemoveResult = FoliageModule.RemoveFoliageInstances(MeshPath, Center, static_cast<float>(Radius));

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (RemoveResult.bSuccess)
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Removed %d foliage instances"), RemoveResult.InstancesRemoved));
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to remove foliage instances: %s"), *RemoveResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
