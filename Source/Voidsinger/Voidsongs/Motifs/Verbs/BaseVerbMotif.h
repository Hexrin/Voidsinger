// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/Motifs/BaseMotif.h"
#include "BaseVerbMotif.generated.h"

/* \/ ============== \/ *\
|  \/ UBaseVerbMotif \/  |
\* \/ ============== \/ */

/**
 * The BaseVerbVoidsong stores an overridable fuction on it. It is the base class for all Verb Voidsongs. When this type of Voidsong is performed, an action will be executed when the parts that were activated
 * interact with something. For example, if the Verb Voidsong "Hot" was played along with the "Laser" noun, all lasers would fire with an extra hot laser. Whatever these lasers hit will be heated up.
 */
UCLASS(Abstract, Blueprintable, config = VoidSettings)
class VOIDSINGER_API UBaseVerbMotif : public UBaseMotif
{
	GENERATED_BODY()
	
	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

public:

	/**
	 * Does the effect of the Verb using the given information.
	 *
	 * @param Instigator - The object that activated this Voidsong
	 * @param Hit - The hit result information
	 * @param Effectiveness - The effectiveness of the activation
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongActivation")
	virtual void ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};

/* /\ ============== /\ *\
|  /\ UBaseVerbMotif /\  |
\* /\ ============== /\ */