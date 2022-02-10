// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseVerbMotif.h"
#include "PushVerbMotif.generated.h"

/**
 * Pushes the ship hit with this Voidsong away from whatever activated this Voidsong
 */
UCLASS()
class VOIDSINGER_API UPushVerbMotif : public UBaseVerbMotif
{
	GENERATED_BODY()
	
	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

	/**
	 * Adds an impulse to the hit ship away from the direction of the instigator using the given information
	 *
	 * @param Instigator - The object that activated this Voidsong
	 * @param Hit - The hit result information
	 * @param Effectiveness - The effectiveness of the activation
	 */
	virtual void ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness) override;

	//The force that this Voidsong will push hit ships with
	UPROPERTY(EditDefaultsOnly)
	float PushForce;

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
