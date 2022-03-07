// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterModule.h"
#include "Voidsinger/Voidgrids/ThrustManager.h"
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
	if (IsValid(GetVoidgrid()))
	{
		if (IsValid(GetVoidgrid()->GetThrustManager()))
		{
			if ((Data.Vector | ThrustDirection) > 0 && GetVoidgrid()->GetThrustManager()->TimeToLinearVelocity(Data.Vector) > 0)
			{
				Thrust();
				return;
			}

			if(GetVoidgrid()->GetThrustManager()->GetThrustRotation())
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s does not have a valid thrust manager"), *GetVoidgrid()->GetName());
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