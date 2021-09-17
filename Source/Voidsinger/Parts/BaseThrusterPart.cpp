// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseThrusterPart.h"

bool UBaseThrusterPart::ThrustInDirection(float ThrustForce, float ThrustDirection)
{
	TArray<FVector4> ThrustRequests = TArray<FVector4>();
	FVector2D ForceNeeded = FVector2D(ThrustForce, 0).GetRotated(ThrustDirection);
	for (FThrusterData Thruster : GetThrustSources())
	{
		float ThrusterAlignment = FVector2D::DotProduct(FVector2D(ThrustForce, 0).GetRotated(ThrustDirection), FVector2D(Thruster.ThrustForce, 0).GetRotated(Thruster.RelativeThrustRotation));
		if (ThrusterAlignment > 0)
		{
			FVector2D Thrust = FVector2D((ThrusterAlignment > 1) ? (Thruster.ThrustForce / ThrusterAlignment) : (ThrusterAlignment * Thruster.ThrustForce), 0).GetRotated(Thruster.RelativeThrustRotation);
			ThrustRequests.Emplace(FVector4(Thrust,Thruster.RelativeThrustLoaction))
		}
	}
	for (FVector4 Request : ThrustRequests)
	{
		Cast<ABaseShip>(GetPartGrid()->GetOwner())->MovementComponent->AddForce(FVector2D(Request.X, Request.Y), FVector2D(Request.Z, Request.W));
	}
	
	return true;
}
