// Fill out your copyright notice in the Description page of Project Settings.


#include "TranslationalThrusterModule.h"

/**
 * Called when the owning part is activated and calls the functionality of this part.
 * Applies an impulse to the voidgrid in an atempt to get it to move in direction.
 */
void UTranslationalThrusterModule::OnActivate_Implementation(const TArray<UBaseVerbMotif*>& Verbs, float Effectiveness, FVector2D Direction)
{
	FVector2D RotatedThrustDirection = ThrustDirection.GetRotated(GetPartTransfrom().Rotation);
	float TargetThrottle = Direction | RotatedThrustDirection;
	if (TargetThrottle > 0)
	{
		GetVoidgrid()->AddImpulse(RotatedThrustDirection, GetPartTransfrom().TransformGirdLocation(ThrustLocation));
		if (bApplyVoidsongEffect)
		{
			
		}
	}
}