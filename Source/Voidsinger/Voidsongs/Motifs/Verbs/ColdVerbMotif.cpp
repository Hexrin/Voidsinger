// Fill out your copyright notice in the Description page of Project Settings.


#include "ColdVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Removes heat from the hit ship using the given information
 *
 * @param Instigator - The object that activated this Voidsong
 * @param Hit - The hit result information
 * @param Effectiveness - The effectiveness of the activation
 */
void UColdVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	Cast<AVoidgrid>(Hit.GetActor())->AddTemperatureAtLocation(Hit.Location, -1 * Effectiveness * ColdToAdd);
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */