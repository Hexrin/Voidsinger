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
 * Applies an impulse to the voidgrid in an attempt to get it to move in the vector direction and face in the rotation direction.
 */
void UThrusterModule::OnActivate_Implementation(const FPartActivationData Data)
{
	if (IsValid(GetVoidgrid()))
	{
		if (IsValid(GetVoidgrid()->GetThrustManager()))
		{
			FThrustSource ThrustSource = FThrustSource(ThrustForce, ThrustDirection, Part->GetTransform().TransformGridLocation(ThrustLocation));
			if (GetVoidgrid()->GetThrustManager()->GetThrustSourceAccelerationInDirection(Data.Vector, ThrustSource) > 0 || GetVoidgrid()->GetThrustManager()->GetThrustSourceAccelerationInRotation(Data.Rotation > 0, ThrustSource) > 0)
			{
				GetVoidgrid()->AddImpulse(ThrustForce * ThrustDirection, ThrustSource.Location);
			}
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