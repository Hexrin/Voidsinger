// Fill out your copyright notice in the Description page of Project Settings.


#include "PullVerbMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Adds an impulse to the hit ship towards the direction of the instigator using the given information
 *
 * @param Instigator - The object that activated this Voidsong
 * @param Hit - The hit result information
 * @param Effectiveness - The effectiveness of the activation
 */
void UPullVerbMotif::ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness)
{
	//                       |-----------------Impulse Direction-----------------|   |----Impulse Magnitude----|
	FVector2D ImpulseVector = FVector2D((Hit.Location - Hit.TraceStart).Normalize() * Effectiveness * PullForce);
	Cast<AVoidgrid>(Hit.GetActor())->AddImpulse(ImpulseVector, FVector2D(Hit.Location));
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */
