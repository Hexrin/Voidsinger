// Fill out your copyright notice in the Description page of Project Settings.


#include "PushVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Adds an impulse to the hit ship away from the direction of the instigator using the given information
 *
 * @param Instigator - The object that activated this Voidsong
 * @param Hit - The hit result information
 * @param Effectiveness - The effectiveness of the activation
 */
void UPushVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	//                       |-----------------Impulse Direction-----------------|   |----Impulse Magnitude----|
	FVector2D ImpulseVector = FVector2D((Hit.TraceStart - Hit.Location).Normalize() * Effectiveness * PushForce);
	Cast<AVoidgrid>(Hit.GetActor())->AddImpulse(ImpulseVector, Hit.Location);
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */