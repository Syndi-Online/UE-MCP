// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/Impl/PhysicsImplModule.h"
#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "EngineUtils.h"
#include "Editor.h"

FPhysicsImplModule::FPhysicsImplModule(IActorModule& InActorModule)
	: ActorModule(InActorModule)
{
}

static AActor* FindActorByName(UWorld* World, const FString& Name)
{
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		if (It->GetActorLabel().Equals(Name, ESearchCase::IgnoreCase) ||
			It->GetName().Equals(Name, ESearchCase::IgnoreCase))
		{
			return *It;
		}
	}
	return nullptr;
}

FPhysicsCollisionResult FPhysicsImplModule::CreatePhysicsCollision(const FString& ActorName, const FString& ShapeType, const FVector& Dimensions, const FVector& Offset)
{
	FPhysicsCollisionResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AActor* Actor = FindActorByName(World, ActorName);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorName);
		return Result;
	}

	UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
	if (!MeshComp || !MeshComp->GetStaticMesh())
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Actor has no static mesh component");
		return Result;
	}

	UBodySetup* BodySetup = MeshComp->GetStaticMesh()->GetBodySetup();
	if (!BodySetup)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Static mesh has no body setup");
		return Result;
	}

	if (ShapeType.Equals(TEXT("box"), ESearchCase::IgnoreCase))
	{
		FKBoxElem BoxElem(Dimensions.X, Dimensions.Y, Dimensions.Z);
		BoxElem.Center = Offset;
		BodySetup->AggGeom.BoxElems.Add(BoxElem);
	}
	else if (ShapeType.Equals(TEXT("sphere"), ESearchCase::IgnoreCase))
	{
		FKSphereElem SphereElem(Dimensions.X);
		SphereElem.Center = Offset;
		BodySetup->AggGeom.SphereElems.Add(SphereElem);
	}
	else if (ShapeType.Equals(TEXT("capsule"), ESearchCase::IgnoreCase))
	{
		FKSphylElem CapsuleElem(Dimensions.X, Dimensions.Y);
		CapsuleElem.Center = Offset;
		BodySetup->AggGeom.SphylElems.Add(CapsuleElem);
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown shape type: %s. Use box, sphere, or capsule."), *ShapeType);
		return Result;
	}

	BodySetup->InvalidatePhysicsData();
	BodySetup->CreatePhysicsMeshes();
	MeshComp->RecreatePhysicsState();

	Result.bSuccess = true;
	Result.ShapesAdded = 1;
	return Result;
}

FPhysicalMaterialResult FPhysicsImplModule::SetPhysicalMaterial(const FString& ActorName, const FString& MaterialPath)
{
	FPhysicalMaterialResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AActor* Actor = FindActorByName(World, ActorName);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorName);
		return Result;
	}

	UPhysicalMaterial* PhysMat = LoadObject<UPhysicalMaterial>(nullptr, *MaterialPath);
	if (!PhysMat)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Physical material not found: %s"), *MaterialPath);
		return Result;
	}

	UPrimitiveComponent* PrimComp = Actor->FindComponentByClass<UPrimitiveComponent>();
	if (!PrimComp)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Actor has no primitive component");
		return Result;
	}

	PrimComp->SetPhysMaterialOverride(PhysMat);

	Result.bSuccess = true;
	return Result;
}

FPhysicsVelocityResult FPhysicsImplModule::SetPhysicsVelocity(const FString& ActorName, const FVector& LinearVelocity, const FVector& AngularVelocity, bool bEnablePhysics)
{
	FPhysicsVelocityResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AActor* Actor = FindActorByName(World, ActorName);
	if (!Actor)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor not found: %s"), *ActorName);
		return Result;
	}

	UPrimitiveComponent* PrimComp = Actor->FindComponentByClass<UPrimitiveComponent>();
	if (!PrimComp)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Actor has no primitive component");
		return Result;
	}

	if (bEnablePhysics)
	{
		PrimComp->SetSimulatePhysics(true);
	}

	PrimComp->SetPhysicsLinearVelocity(LinearVelocity);
	PrimComp->SetPhysicsAngularVelocityInDegrees(AngularVelocity);

	Result.bSuccess = true;
	return Result;
}

FPhysicsConstraintResult FPhysicsImplModule::ManageConstraints(const FString& ActorName1, const FString& ActorName2, const FString& ConstraintType, bool bDisableCollision)
{
	FPhysicsConstraintResult Result;

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("No editor world available");
		return Result;
	}

	AActor* Actor1 = FindActorByName(World, ActorName1);
	if (!Actor1)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor 1 not found: %s"), *ActorName1);
		return Result;
	}

	AActor* Actor2 = FindActorByName(World, ActorName2);
	if (!Actor2)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Actor 2 not found: %s"), *ActorName2);
		return Result;
	}

	UPrimitiveComponent* Comp1 = Actor1->FindComponentByClass<UPrimitiveComponent>();
	UPrimitiveComponent* Comp2 = Actor2->FindComponentByClass<UPrimitiveComponent>();
	if (!Comp1 || !Comp2)
	{
		Result.bSuccess = false;
		Result.ErrorMessage = TEXT("Both actors must have primitive components");
		return Result;
	}

	UPhysicsConstraintComponent* ConstraintComp = NewObject<UPhysicsConstraintComponent>(Actor1, NAME_None, RF_Transactional);
	ConstraintComp->SetWorldLocation(Actor1->GetActorLocation());
	ConstraintComp->AttachToComponent(Actor1->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	ConstraintComp->RegisterComponent();

	ConstraintComp->SetConstrainedComponents(Comp1, NAME_None, Comp2, NAME_None);

	if (ConstraintType.Equals(TEXT("weld"), ESearchCase::IgnoreCase) || ConstraintType.Equals(TEXT("fixed"), ESearchCase::IgnoreCase))
	{
		ConstraintComp->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		ConstraintComp->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		ConstraintComp->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	}
	else if (ConstraintType.Equals(TEXT("hinge"), ESearchCase::IgnoreCase))
	{
		ConstraintComp->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		ConstraintComp->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		ConstraintComp->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.f);
	}
	else if (ConstraintType.Equals(TEXT("ball"), ESearchCase::IgnoreCase))
	{
		ConstraintComp->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.f);
		ConstraintComp->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.f);
		ConstraintComp->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.f);
		ConstraintComp->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.f);
	}
	else if (ConstraintType.Equals(TEXT("free"), ESearchCase::IgnoreCase))
	{
		ConstraintComp->SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 0.f);
		ConstraintComp->SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 0.f);
		ConstraintComp->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0.f);
		ConstraintComp->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.f);
		ConstraintComp->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.f);
		ConstraintComp->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.f);
	}
	else
	{
		Result.bSuccess = false;
		Result.ErrorMessage = FString::Printf(TEXT("Unknown constraint type: %s. Use weld, hinge, ball, or free."), *ConstraintType);
		return Result;
	}

	ConstraintComp->SetDisableCollision(bDisableCollision);

	Result.bSuccess = true;
	Result.ConstraintName = ConstraintComp->GetName();
	return Result;
}
