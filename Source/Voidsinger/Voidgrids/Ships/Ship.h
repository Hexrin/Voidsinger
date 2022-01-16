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
	 * 
	 */
	void PlayVoidsong(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
