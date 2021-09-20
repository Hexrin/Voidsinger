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

const FVector2D UBaseThrusterPart::GetThrustLocation()
{
	return ThrustLocation + GetLocation();
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
		Cast<ABaseShip>(GetPartGrid()->GetOwner())->PhysicsComponent->AddForce(FVector2D(GetLocation()) + ThrustLocation.GetRotated(GetRotation()), FVector2D(ThrustForce, 0).GetRotated(GetRotation() + ThrustRotation) * DeltaTime);
		bIsThrusting = false;
	}
}

