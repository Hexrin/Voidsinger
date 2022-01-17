// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "Ship.generated.h"

/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UBaseMotif;
class UBaseFactionMotif;
class UBaseNounMotif;
class UBaseVerbMotif;

/* /\ Forward Declarations /\ *\
\* ------------------------- */

/**
 * A controllable Voidgrid.
 * Can receive input and handles thrust control.
 */
UCLASS()
class VOIDSINGER_API AShip : public AVoidgrid
{
	GENERATED_BODY()

	/* ---------------------------------- *\
	\* \/ Playable Voidsong Management \/ */

public:

	/**
	 * Adds Voidsongs to the list of Voidsongs that this component can play.
	 *
	 * @param VoidsongsAdded - The new playable Voidsongs
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayableVoidsongManagement")
	void AddNewMotifs(TArray<UBaseMotif*> MotifsAdded);

protected:

	//Stores the Voidsongs that this component can play.
	TSet<UBaseMotif*> PlayableMotifs;

	/* /\ Playable Voidsong Management /\ *\
	\* ---------------------------------- */

	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

	/**
	 * Plays a Voidsong with the given Motifs
	 * 
	 * @param Factions - The Faction Motifs played
	 * @param Nouns - The Noun Motifs played
	 * @param Verbs - The Verb Motifs played
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongActivation")
	void PlayVoidsong(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
