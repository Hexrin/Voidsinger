// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterModule.h"

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
	FVector2D RotatedThrustDirection = UPartRotationFunctions::RotateLocation(ThrustDirection, Part->GetTransform().Rotation);

	//if (ThrusterModules.IsEmpty())
	//{
	//}
}

/**
 * Updates voidgrid thrust capabilities.
 * 
 * @param bApplyChangeEffect - Whether to apply the effects of this change
 */
void UThrusterModule::FunctionalityRestored(bool bApplyChangeEffect)
{

}

/**
 * Updates voidgrid thrust capabilities.
 * 
 * @param bApplyChangeEffect - Whether to apply the effects of this change
 */
void UThrusterModule::FunctionalityLost(bool bApplyChangeEffect)
{

}