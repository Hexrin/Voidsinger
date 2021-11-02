// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseThrusterPart.h"
#include "Voidsinger/Ships/BaseShip.h"

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
	return ThrustRotation + GetRelativeRotation();
}

const FVector2D UBaseThrusterPart::GetThrustRelativeLocation()
{
	return ThrustLocation.GetRotated(GetRelativeRotation()) + FVector2D(GetPartRelativeLocation());
}

const bool UBaseThrusterPart::IsThrusting()
{
	return Throttle != 0;
}

const float UBaseThrusterPart::GetThrottle()
{
	return Throttle;
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
		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddImpulse(FVector2D(ThrustForce * Throttle * FMath::Clamp(DeltaTime, 0.f, 0.0666666667f), 0).GetRotated(GetThrustRotation() + GetShip()->GetActorRotation().Yaw), FVector2D(GetShip()->GetActorQuat().RotateVector(FVector(GetThrustRelativeLocation(), 0))));
		Throttle = 0;
	}
}

