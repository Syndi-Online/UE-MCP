// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/ManageConstraintsImplTool.h"
#include "Modules/Interfaces/IPhysicsModule.h"
#include "Dom/JsonValue.h"

FManageConstraintsImplTool::FManageConstraintsImplTool(IPhysicsModule& InPhysicsModule)
	: PhysicsModule(InPhysicsModule)
{
}

FString FManageConstraintsImplTool::GetName() const
{
	return TEXT("manage_constraints");
}

FString FManageConstraintsImplTool::GetDescription() const
{
	return TEXT("Create a physics constraint between two actors");
}

TSharedPtr<FJsonObject> FManageConstraintsImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorName1Prop = MakeShared<FJsonObject>();
	ActorName1Prop->SetStringField(TEXT("type"), TEXT("string"));
	ActorName1Prop->SetStringField(TEXT("description"), TEXT("Name of the first actor"));
	Properties->SetObjectField(TEXT("actor_name_1"), ActorName1Prop);

	TSharedPtr<FJsonObject> ActorName2Prop = MakeShared<FJsonObject>();
	ActorName2Prop->SetStringField(TEXT("type"), TEXT("string"));
	ActorName2Prop->SetStringField(TEXT("description"), TEXT("Name of the second actor"));
	Properties->SetObjectField(TEXT("actor_name_2"), ActorName2Prop);

	TSharedPtr<FJsonObject> ConstraintTypeProp = MakeShared<FJsonObject>();
	ConstraintTypeProp->SetStringField(TEXT("type"), TEXT("string"));
	ConstraintTypeProp->SetStringField(TEXT("description"), TEXT("Type of constraint: weld, hinge, ball, or free"));
	Properties->SetObjectField(TEXT("constraint_type"), ConstraintTypeProp);

	TSharedPtr<FJsonObject> DisableCollisionProp = MakeShared<FJsonObject>();
	DisableCollisionProp->SetStringField(TEXT("type"), TEXT("boolean"));
	DisableCollisionProp->SetStringField(TEXT("description"), TEXT("Disable collision between constrained actors (defaults to true)"));
	Properties->SetObjectField(TEXT("disable_collision"), DisableCollisionProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_name_1")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_name_2")));
	Required.Add(MakeShared<FJsonValueString>(TEXT("constraint_type")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FManageConstraintsImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorName1;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_name_1"), ActorName1))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_name_1"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ActorName2;
	if (!Arguments->TryGetStringField(TEXT("actor_name_2"), ActorName2))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_name_2"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	FString ConstraintType;
	if (!Arguments->TryGetStringField(TEXT("constraint_type"), ConstraintType))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: constraint_type"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional disable_collision (defaults to true)
	bool bDisableCollision = true;
	Arguments->TryGetBoolField(TEXT("disable_collision"), bDisableCollision);

	FPhysicsConstraintResult ConstraintResult = PhysicsModule.ManageConstraints(ActorName1, ActorName2, ConstraintType, bDisableCollision);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (ConstraintResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Created %s constraint: %s"),
			*ConstraintType, *ConstraintResult.ConstraintName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to create constraint: %s"), *ConstraintResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
