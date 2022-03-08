// Fill out your copyright notice in the Description page of Project Settings.


#include "HotVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* \/ ============= \/ *\
|  \/ UHotVerbMotif \/  |
\* \/ ============= \/ */

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Adds heat to the hit ship using the given information
 *
 * @param Instigator - The object that activated this Voidsong
 * @param Hit - The hit result information
 * @param Effectiveness - The effectiveness of the activation
 */
void UHotVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	Cast<AVoidgrid>(Hit.GetActor())->AddTemperatureAtLocation(Hit.Location, Effectiveness * HeatToAdd);
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */

/* /\ ============= /\ *\
|  /\ UHotVerbMotif /\  |
\* /\ ============= /\ */