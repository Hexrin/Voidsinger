// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterModule.h"
#include "ThrustManager.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/**
 * Initializes the part module's protected and private variables
 *
 * @param OwningPart - The part that this module is being initialized from
 */
void UThrusterModule::InitializeVariables(UPart* OwningPart)
{
	Super::InitializeVariables(OwningPart);
}

/**
 * Called when the owning part is activated and calls the functionality of this part.
 * Applies an impulse to the voidgrid in an atempt to get it to move in teh vector dirction and face in the roatation direction.
 */
void UThrusterModule::OnActivate_Implementation(const FPartActivationData Data)
{
	if (IsValid(Part))
	{
		FVector2D RotatedThrustDirection = UPartRotationFunctions::RotateLocation(ThrustDirection, Part->GetTransform().Rotation);
		bool ClockwiseRotation = true;
		if ((FVector2D::CrossProduct((Data.Vector).GetSafeNormal(), ((RotatedThrustDirection).GetSafeNormal()))) > 0.f)
		{
			ClockwiseRotation = true;
		}
		else
		{
			ClockwiseRotation = false;
		}

		if ((GetVoidgrid()->GetThrustManager()->GetThrustDirection(RotatedThrustDirection, FThrustSource(1.f, RotatedThrustDirection, Data.Vector))) > (FVector2D(0, 0)) || (GetVoidgrid()->GetThrustManager()->GetThrustRotation(ClockwiseRotation, FThrustSource(1.f, RotatedThrustDirection, Data.Vector))) > 0.f)
		{
			ActivateWithEffectivenessVectorAndRotation(1, Part->GetTransform().Location, float(Part->GetTransform().Rotation));
		}
	}
}

/**
 * Updates voidgrid thrust capabilities.
 */
void UThrusterModule::FunctionalityRestored()
{

}

/**
 * Updates voidgrid thrust capabilities.
 */
void UThrusterModule::FunctionalityLost()
{

}