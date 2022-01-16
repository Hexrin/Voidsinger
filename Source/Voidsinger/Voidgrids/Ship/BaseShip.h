// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "BaseShip.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API ABaseShip : public AVoidgrid
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
	void AddNewVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> VoidsongsAdded);

protected:

	//Stores the Voidsongs that this component can play.
	TSet<TSubclassOf<UBaseVoidsong>> PlayableVoidsongs;

	/* /\ Playable Voidsong Management /\ *\
	\* ---------------------------------- */

	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

	/**
	 * 
	 */
	void BroadcastPlayVoidsongs(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
