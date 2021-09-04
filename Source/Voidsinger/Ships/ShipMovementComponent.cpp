// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"
#include "BaseShip.h"

// Sets default values for this component's properties
UShipMovementComponent::UShipMovementComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	Ship = (ABaseShip*)GetOwner();
	AngularVelocity = 0;
	Velocity = FVector2D(0, 0);

}


// Called when the game starts
void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


//Called every tick
void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//transform the ship by the velocity and angular velocity
	Ship->SetActorLocation(Ship->GetActorLocation() + FVector(Velocity.X, Velocity.Y, 0) * DeltaTime);
	Ship->SetActorRotation(Ship->GetActorRotation() + FRotator(0, AngularVelocity, 0) * DeltaTime);

}

//When a force is added to the ship
void UShipMovementComponent::AddForce(FVector2D ForceLocation, FVector2D Force)
{
	///UE_LOG(LogTemp, Warning, TEXT("???? x=%f, y=%f"), Ship->PartGrid->GetCenterOfMass().X, Ship->PartGrid->GetCenterOfMass().Y)
	//Distance vector is the distance from the center of mass to the force location.
	FVector2D DistanceVector = Ship->PartGrid->GetCenterOfMass() - ForceLocation;
	
	//Account for exactly hitting the center of mass, in which case there would be no rotation and the full 
	//force would be used.
	if (DistanceVector.IsNearlyZero())
	{
		Velocity += Force / Ship->PartGrid->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("????????"));
	}

	//calculate the change in velocity and change in angular velocity based off the force
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("dot=%f, dist x=%f, y=%f"), FVector2D::DotProduct(DistanceVector.GetSafeNormal(), Force), DistanceVector.GetSafeNormal().X, DistanceVector.GetSafeNormal().Y);
		//these 2 lines of math took literally 7 hours to figure out.
		Velocity += (FVector2D::DotProduct(DistanceVector.GetSafeNormal(), Force) * DistanceVector.GetSafeNormal()) / Ship->PartGrid->GetMass();
		AngularVelocity -= FVector2D::DotProduct(DistanceVector.GetRotated(90).GetSafeNormal(), Force) * DistanceVector.Size() / Ship->PartGrid->GetMass();
	}

	
}

