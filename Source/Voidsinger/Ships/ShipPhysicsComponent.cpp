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
	
	if (!UFunctionLibrary::SetActorTransformSweepComponets(Ship, Result, Comps, Ship->GetActorTransform(), Ship->GetActorTransform() + FTransform(FRotator(0, AngularVelocity, 0) * DeltaTime, FVector(LinearVelocity.X, LinearVelocity.Y, 0) * DeltaTime, FVector())))
	{
		UE_LOG(LogTemp, Warning, TEXT("SweepFaild"));
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

void UShipPhysicsComponent::SetVelocityDirectly(FVector2D NewVelocity)
{
	LinearVelocity = NewVelocity;
}

void UShipPhysicsComponent::UpdateMassCalculations()
{
	Mass = Ship->PartGrid->GetMass();
	CenterOfMass = Ship->PartGrid->GetCenterOfMass();
	MomentOfInertia = Ship->PartGrid->GetMomentOfInertia();
	FVector DeltaPos = FVector();

	Comps = TArray<UPrimitiveComponent*>();
	for (UActorComponent* Comp : Ship->GetComponentsByClass(UPrimitiveComponent::StaticClass()))
	{
		Comps.Emplace(Cast<UPrimitiveComponent>(Comp));
	}

	bool HasSetDeltaPos = false;
	for (auto& Component : Ship->PartGrid->GetPartGrid())
	{
		FVector NewLoc = FVector(-1 * CenterOfMass + Component.Key, 0);
		if (NewLoc != FVector() && !HasSetDeltaPos)
		{
			HasSetDeltaPos = true;
			DeltaPos = Component.Value.PixelMesh->GetRelativeLocation() - NewLoc;
		}
		Component.Value.PixelMesh->SetRelativeLocation(NewLoc);
	}

	Ship->SetActorLocation(Ship->GetActorLocation() + DeltaPos);
}

float UShipPhysicsComponent::GetAngularVelocity()
{
	return AngularVelocity;
}