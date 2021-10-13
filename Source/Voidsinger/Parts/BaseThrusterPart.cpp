// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseThrusterPart.h"
UBaseThrusterPart::UBaseThrusterPart()
{
	ThrustForce = 10;
	ThrustLocation = FVector2D();
	ThrustRotation = 0;
	bIsThrusting = false;
}
const float UBaseThrusterPart::GetThrustForce()
{
	return ThrustForce;
}

const float UBaseThrusterPart::GetThrustRotation()
{
	return ThrustRotation + GetRotation();
}

const FVector2D UBaseThrusterPart::GetThrustRelativeLocation()
{
	return ThrustLocation.GetRotated(GetRotation()) + FVector2D(GetPartRelativeLocation());
}

const bool UBaseThrusterPart::IsThrusting()
{
	return bIsThrusting;
}

void UBaseThrusterPart::Thrust()
{
	bIsThrusting = true;
}

void UBaseThrusterPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsThrusting)
	{
		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddImpulse(FVector2D(ThrustForce, 0).GetRotated(GetThrustRotation() + GetShip()->GetActorRotation().Yaw) * DeltaTime, FVector2D(GetShip()->GetActorQuat().RotateVector(FVector(GetThrustRelativeLocation(), 0))));
		bIsThrusting = false;
	}
}

