// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPhysicsComponent.h"
#include "BaseShip.h"

// Sets default values for this component's properties
UShipPhysicsComponent::UShipPhysicsComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	Ship = (ABaseShip*)GetOwner();
	AngularVelocity = 0;
	LinearVelocity = FVector2D(0, 0);

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
	if (!DeltaVelocity.IsNearlyZero(DeltaTime / 25))
	{
		FVector2D LeverDirection = (Ship->PartGrid->GetCenterOfMass() - CenterOfThrust).GetSafeNormal();
		if (LeverDirection.IsNearlyZero())
		{
			LeverDirection = DeltaVelocity.GetSafeNormal();
		}
		//UE_LOG(LogTemp, Warning, TEXT("line dot=%f"), FVector2D::DotProduct(LeverDirection, DeltaVelocity));

		LinearVelocity += ((FVector2D::DotProduct(LeverDirection, DeltaVelocity) * LeverDirection) / Ship->PartGrid->GetMass()).GetRotated(GetOwner()->GetActorRotation().Yaw);
		AngularVelocity -= FVector2D::DotProduct(LeverDirection.GetRotated(90), DeltaVelocity) * (Ship->PartGrid->GetCenterOfMass() - CenterOfThrust).Size() / Ship->PartGrid->GetMass();
		//transform the ship by the velocity and angular velocity
		
	}
	Ship->SetActorLocation(Ship->GetActorLocation() + FVector(LinearVelocity.X, LinearVelocity.Y, 0) * DeltaTime);
	Ship->SetActorRotation(Ship->GetActorRotation() + FRotator(0, AngularVelocity, 0) * DeltaTime);

	DeltaVelocity.Set(0, 0);
	CenterOfThrust.Set(0, 0);
}

//When a force is added to the ship
void UShipPhysicsComponent::AddForce(FVector2D RelativeForceLocation, FVector2D RelativeForce)
{
	if (!RelativeForce.IsZero())
	{
		float VelocityShare = RelativeForce.Size();
		VelocityShare = VelocityShare / (VelocityShare + DeltaVelocity.Size());
		DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation() + FVector(RelativeForceLocation.GetRotated(GetOwner()->GetActorRotation().Yaw), 0), (GetOwner()->GetActorLocation() + FVector(RelativeForceLocation.GetRotated(GetOwner()->GetActorRotation().Yaw), 0)) + FVector(RelativeForce.GetRotated(GetOwner()->GetActorRotation().Yaw), 0) * 10, FColor::Emerald, false, -1.0F, 0U, 1);


		CenterOfThrust = (1 - VelocityShare) * CenterOfThrust + RelativeForceLocation * VelocityShare;

		UE_LOG(LogTemp, Warning, TEXT("share =%f"), VelocityShare);
		DeltaVelocity += RelativeForce;
	}
}

FVector2D UShipPhysicsComponent::GetVelocity()
{
	return LinearVelocity;
}

float UShipPhysicsComponent::GetAngularVelocity()
{
	return AngularVelocity;
}