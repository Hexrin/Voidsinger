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


	LinearVelocity += ((FVector2D::DotProduct(DistanceVector.GetSafeNormal(), DeltaVelocity) * DistanceVector.GetSafeNormal()) / Ship->PartGrid->GetMass()).GetRotated(GetOwner()->GetActorRotation().Yaw);
	AngularVelocity -= FVector2D::DotProduct(DistanceVector.GetRotated(90).GetSafeNormal(), DeltaVelocity) * DistanceVector.Size() / Ship->PartGrid->GetMass();
	//transform the ship by the velocity and angular velocity
	Ship->SetActorLocation(Ship->GetActorLocation() + FVector(LinearVelocity.X, LinearVelocity.Y, 0) * DeltaTime);
	Ship->SetActorRotation(Ship->GetActorRotation() + FRotator(0, AngularVelocity, 0) * DeltaTime);


	DeltaVelocity.Set(0, 0);
	CenterOfThrust.Set(0, 0);
}

//When a force is added to the ship
void UShipMovementComponent::AddForce(FVector2D ForceLocation, FVector2D Force)
{
	FVector2D VelocityShare = (Force/(Force + DeltaVelocity));
	CenterOfThrust = VelocityShare;


	DeltaVelocity += Force.GetRotated(-1*GetOwner()->GetActorRotation().Yaw);
	
}

FVector2D UShipMovementComponent::GetVelocity()
{
	return LinearVelocity;
}

float UShipMovementComponent::GetAngularVelocity()
{
	return AngularVelocity;
}