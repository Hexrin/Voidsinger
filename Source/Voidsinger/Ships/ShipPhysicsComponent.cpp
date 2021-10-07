// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPhysicsComponent.h"
#include "BaseShip.h"
#include "Voidsinger/Parts/BasePart.h"

// Sets default values for this component's properties
UShipPhysicsComponent::UShipPhysicsComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	Ship = (ABaseShip*)GetOwner();
	AngularVelocity = 0;
	LinearVelocity = FVector2D(0, 0);
	DeltaLinearVelocity = FVector2D(0, 0);
	DeltaAngularVelocity = 0;
}


// Called when the game starts
void UShipPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


//Called every tick
void UShipPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LinearVelocity += DeltaLinearVelocity.GetRotated(Ship->GetActorRotation().Yaw) * DeltaTime;
	AngularVelocity += DeltaAngularVelocity * DeltaTime;

	FHitResult Result = FHitResult();
	TArray<UPrimitiveComponent*> Comps = TArray<UPrimitiveComponent*>();
	for (UActorComponent* Comp : Ship->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		Comps.Emplace(Cast<UPrimitiveComponent>(Comp));
	}
	if (UFunctionLibrary::SetActorTransformSweepComponets(Ship, Result, Comps, Ship->GetActorTransform(), Ship->GetActorTransform() + FTransform(FRotator(0, AngularVelocity, 0) * DeltaTime, FVector(LinearVelocity.X, LinearVelocity.Y, 0) * DeltaTime, FVector())))
	{

	}
	//Ship->SetActorRotation(Ship->GetActorRotation() + FRotator(0, AngularVelocity, 0) * DeltaTime);

	DeltaLinearVelocity = FVector2D(0, 0);
	DeltaAngularVelocity = 0;

}

//When a force is added to the ship
void UShipPhysicsComponent::AddForce(FVector2D RelativeForceLocation, FVector2D RelativeForce)
{
	if (!RelativeForce.IsZero())
	{
		RelativeForceLocation -= CenterOfMass;


		DeltaLinearVelocity += RelativeForce / Mass;
		DeltaAngularVelocity += (RelativeForceLocation.X * RelativeForce.Y - RelativeForceLocation.Y * RelativeForce.X) / MomentOfInertia;
	}
}

FVector2D UShipPhysicsComponent::GetVelocity()
{
	return LinearVelocity;
}

void UShipPhysicsComponent::UpdateMassCalculations()
{
	Mass = Ship->PartGrid->GetMass();
	CenterOfMass = Ship->PartGrid->GetCenterOfMass();
	MomentOfInertia = Ship->PartGrid->GetMomentOfInertia();

	for (auto& Component : Ship->PartGrid->GetPartGrid())
	{
		Component.Value.PixelMesh->SetRelativeLocation(FVector(-1 * CenterOfMass + Component.Key, 0));
	}
}

float UShipPhysicsComponent::GetAngularVelocity()
{
	return AngularVelocity;
}