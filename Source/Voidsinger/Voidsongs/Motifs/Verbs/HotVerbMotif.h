// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseVerbMotif.h"
#include "HotVerbMotif.generated.h"

/**
 * Heats up the ship hit with this Motif
 */
UCLASS()
class VOIDSINGER_API UHotVerbMotif : public UBaseVerbMotif
{
	GENERATED_BODY()
	
	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

	/**
	 * Adds heat to the hit ship using the given information
	 *
	 * @param Instigator - The object that activated this Voidsong
	 * @param Hit - The hit result information
	 * @param Effectiveness - The effectiveness of the activation
	 */
	virtual void ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness) override;

	UPROPERTY(EditDefaultsOnly)
	float HeatToAdd;

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
