// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovementComponent.h"

// Sets default values for this component's properties
UShipMovementComponent::UShipMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Ship = (ABaseShip*)GetOwner();
	AngularVelocity = 0;
	Velocity = FVector2D(0, 0);
	// ...
}


// Called when the game starts
void UShipMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



void UShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Ship->SetActorLocation(Ship->GetActorLocation() + FVector(Velocity.X, Velocity.Y, 0));
	Ship->SetActorRotation(Ship->GetActorRotation() + FRotator(0, AngularVelocity, 0));

}

void UShipMovementComponent::AddForce(FVector2D ForceLocation, FVector2D Force)
{
	
	FVector2D DistanceVector = Ship->GetCenterOfMass() - ForceLocation;

	if (DistanceVector.IsNearlyZero())
	{
		Velocity += Force / Ship->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("????????"));
	}
	else
	{
		Velocity += (FVector2D::DotProduct(DistanceVector.GetSafeNormal(), Force) * DistanceVector.GetSafeNormal()) / Ship->GetMass();
		AngularVelocity += FVector2D::DotProduct(DistanceVector.GetRotated(90).GetSafeNormal(), Force) * DistanceVector.Size() / Ship->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("mass = %f"), Ship->GetMass());
	}

	//UE_LOG(LogTemp, Warning, TEXT("x = %f, y = %f"), DistanceVector.GetSafeNormal().X * FVector2D::DotProduct(DistanceVector.GetSafeNormal(), Force), DistanceVector.GetSafeNormal().Y * FVector2D::DotProduct(DistanceVector.GetSafeNormal(), Force));
}

