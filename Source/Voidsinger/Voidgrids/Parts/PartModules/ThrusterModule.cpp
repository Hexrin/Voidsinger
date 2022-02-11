// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterModule.h"

#include "Voidsinger/Voidgrids/Voidgrid.h"

float UThrusterModule::TimeToLinearVelocity(AVoidgrid* Target, FVector2D Velocity)
{
	return 0;
}

float UThrusterModule::TimeToLocation(AVoidgrid* Target, FVector2D Location)
{
	return 0;
}

float UThrusterModule::TimeToAngularVelocity(AVoidgrid* Target, float Velocity)
{
	return 0;
}

float UThrusterModule::TimeToOrientation(AVoidgrid* Target, float Orientation)
{
	return 0;
}


/**
 * Called when the owning part is activated and calls the functionality of this part.
 * Applies an impulse to the voidgrid in an atempt to get it to move in teh vector dirction and face in the roatation direction.
 */
void UThrusterModule::OnActivate_Implementation(const FPartActivationData Data)
{
	FVector2D RotatedThrustDirection = UPartRotationFunctions::RotateLocation(ThrustDirection, Part->GetTransform().Rotation);

	//if (ThrusterModules.IsEmpty())
	//{
	//}
}

/**
 * Updates voidgrid thrust capabilities.
 */
void UThrusterModule::FunctionalityRestored()
{
	GetVoidgrid()->ThrusterModules.Add(this);
}

/**
 * Updates voidgrid thrust capabilities.
 */
void UThrusterModule::FunctionalityLost()
{
	GetVoidgrid()->ThrusterModules.Remove(this);
}
