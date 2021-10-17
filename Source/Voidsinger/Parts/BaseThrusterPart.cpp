// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseThrusterPart.h"
UBaseThrusterPart::UBaseThrusterPart()
{
	ThrustForce = 10;
	ThrustLocation = FVector2D();
	ThrustRotation = 0;
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
	return Throttle != 0;
}

void UBaseThrusterPart::Thrust(float NewThrottle)
{
	Throttle = FMath::Clamp(NewThrottle, 0.f, 1.f);
}

void UBaseThrusterPart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Throttle != 0)
	{
		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddImpulse(FVector2D(ThrustForce * Throttle, 0).GetRotated(GetThrustRotation() + GetShip()->GetActorRotation().Yaw) * DeltaTime, FVector2D(GetShip()->GetActorQuat().RotateVector(FVector(GetThrustRelativeLocation(), 0))));
		Throttle = 0;
	}
}

