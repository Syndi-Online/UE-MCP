// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tools/Impl/SetPhysicsVelocityImplTool.h"
#include "Modules/Interfaces/IPhysicsModule.h"
#include "Dom/JsonValue.h"

FSetPhysicsVelocityImplTool::FSetPhysicsVelocityImplTool(IPhysicsModule& InPhysicsModule)
	: PhysicsModule(InPhysicsModule)
{
}

FString FSetPhysicsVelocityImplTool::GetName() const
{
	return TEXT("set_physics_velocity");
}

FString FSetPhysicsVelocityImplTool::GetDescription() const
{
	return TEXT("Set linear and angular velocity on an actor's physics body");
}

TSharedPtr<FJsonObject> FSetPhysicsVelocityImplTool::GetInputSchema() const
{
	TSharedPtr<FJsonObject> Schema = MakeShared<FJsonObject>();
	Schema->SetStringField(TEXT("type"), TEXT("object"));

	TSharedPtr<FJsonObject> Properties = MakeShared<FJsonObject>();

	TSharedPtr<FJsonObject> ActorNameProp = MakeShared<FJsonObject>();
	ActorNameProp->SetStringField(TEXT("type"), TEXT("string"));
	ActorNameProp->SetStringField(TEXT("description"), TEXT("Name of the actor"));
	Properties->SetObjectField(TEXT("actor_name"), ActorNameProp);

	TSharedPtr<FJsonObject> NumProp = MakeShared<FJsonObject>();
	NumProp->SetStringField(TEXT("type"), TEXT("number"));

	// Linear velocity
	TSharedPtr<FJsonObject> LinearVelProp = MakeShared<FJsonObject>();
	LinearVelProp->SetStringField(TEXT("type"), TEXT("object"));
	LinearVelProp->SetStringField(TEXT("description"), TEXT("Linear velocity in cm/s (defaults to 0,0,0)"));
	TSharedPtr<FJsonObject> LinearVelProps = MakeShared<FJsonObject>();
	LinearVelProps->SetObjectField(TEXT("x"), NumProp);
	LinearVelProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	LinearVelProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	LinearVelProp->SetObjectField(TEXT("properties"), LinearVelProps);
	Properties->SetObjectField(TEXT("linear_velocity"), LinearVelProp);

	// Angular velocity
	TSharedPtr<FJsonObject> AngularVelProp = MakeShared<FJsonObject>();
	AngularVelProp->SetStringField(TEXT("type"), TEXT("object"));
	AngularVelProp->SetStringField(TEXT("description"), TEXT("Angular velocity in degrees/s (defaults to 0,0,0)"));
	TSharedPtr<FJsonObject> AngularVelProps = MakeShared<FJsonObject>();
	AngularVelProps->SetObjectField(TEXT("x"), MakeShared<FJsonObject>(*NumProp));
	AngularVelProps->SetObjectField(TEXT("y"), MakeShared<FJsonObject>(*NumProp));
	AngularVelProps->SetObjectField(TEXT("z"), MakeShared<FJsonObject>(*NumProp));
	AngularVelProp->SetObjectField(TEXT("properties"), AngularVelProps);
	Properties->SetObjectField(TEXT("angular_velocity"), AngularVelProp);

	// Enable physics
	TSharedPtr<FJsonObject> EnablePhysicsProp = MakeShared<FJsonObject>();
	EnablePhysicsProp->SetStringField(TEXT("type"), TEXT("boolean"));
	EnablePhysicsProp->SetStringField(TEXT("description"), TEXT("Enable physics simulation before setting velocity (defaults to true)"));
	Properties->SetObjectField(TEXT("enable_physics"), EnablePhysicsProp);

	Schema->SetObjectField(TEXT("properties"), Properties);

	TArray<TSharedPtr<FJsonValue>> Required;
	Required.Add(MakeShared<FJsonValueString>(TEXT("actor_name")));
	Schema->SetArrayField(TEXT("required"), Required);

	return Schema;
}

TSharedPtr<FJsonObject> FSetPhysicsVelocityImplTool::Execute(const TSharedPtr<FJsonObject>& Arguments)
{
	TSharedPtr<FJsonObject> Result = MakeShared<FJsonObject>();
	TArray<TSharedPtr<FJsonValue>> ContentArray;

	FString ActorName;
	if (!Arguments.IsValid() || !Arguments->TryGetStringField(TEXT("actor_name"), ActorName))
	{
		TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
		TextContent->SetStringField(TEXT("type"), TEXT("text"));
		TextContent->SetStringField(TEXT("text"), TEXT("Missing required parameter: actor_name"));
		ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
		Result->SetArrayField(TEXT("content"), ContentArray);
		Result->SetBoolField(TEXT("isError"), true);
		return Result;
	}

	// Parse optional linear velocity
	FVector LinearVel = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* LinearVelObj;
	if (Arguments->TryGetObjectField(TEXT("linear_velocity"), LinearVelObj))
	{
		(*LinearVelObj)->TryGetNumberField(TEXT("x"), LinearVel.X);
		(*LinearVelObj)->TryGetNumberField(TEXT("y"), LinearVel.Y);
		(*LinearVelObj)->TryGetNumberField(TEXT("z"), LinearVel.Z);
	}

	// Parse optional angular velocity
	FVector AngularVel = FVector::ZeroVector;
	const TSharedPtr<FJsonObject>* AngularVelObj;
	if (Arguments->TryGetObjectField(TEXT("angular_velocity"), AngularVelObj))
	{
		(*AngularVelObj)->TryGetNumberField(TEXT("x"), AngularVel.X);
		(*AngularVelObj)->TryGetNumberField(TEXT("y"), AngularVel.Y);
		(*AngularVelObj)->TryGetNumberField(TEXT("z"), AngularVel.Z);
	}

	// Parse optional enable_physics (defaults to true)
	bool bEnablePhysics = true;
	Arguments->TryGetBoolField(TEXT("enable_physics"), bEnablePhysics);

	FPhysicsVelocityResult VelocityResult = PhysicsModule.SetPhysicsVelocity(ActorName, LinearVel, AngularVel, bEnablePhysics);

	TSharedPtr<FJsonObject> TextContent = MakeShared<FJsonObject>();
	TextContent->SetStringField(TEXT("type"), TEXT("text"));

	if (VelocityResult.bSuccess)
	{
		FString ResponseText = FString::Printf(
			TEXT("Physics velocity set on actor: %s"),
			*ActorName);
		TextContent->SetStringField(TEXT("text"), ResponseText);
		Result->SetBoolField(TEXT("isError"), false);
	}
	else
	{
		TextContent->SetStringField(TEXT("text"),
			FString::Printf(TEXT("Failed to set physics velocity: %s"), *VelocityResult.ErrorMessage));
		Result->SetBoolField(TEXT("isError"), true);
	}

	ContentArray.Add(MakeShared<FJsonValueObject>(TextContent));
	Result->SetArrayField(TEXT("content"), ContentArray);

	return Result;
}
