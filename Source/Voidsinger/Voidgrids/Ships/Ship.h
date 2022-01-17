// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseMotif.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "Ship.generated.h"

/**
 * A controllable Voidgrid.
 * Can recive input and handels thrust control.
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
	void AddNewMotifs(TArray<TSubclassOf<UBaseMotif>> MotifsAdded);

protected:

	//Stores the Voidsongs that this component can play.
	TSet<TSubclassOf<UBaseMotif>> PlayableMotifs;

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
	void PlayVoidsong(const TArray<TSubclassOf<UBaseFactionMotif>>& Factions, const TArray<TSubclassOf<UBaseNounMotif>>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
