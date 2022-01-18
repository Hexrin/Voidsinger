// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "GameFramework/SaveGame.h"
#include "Ship.generated.h"

/* \/ ==== \/ *\
|  \/ Ship \/  |
\* \/ ==== \/ */

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
	void PlayVoidsongs(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */

	/* ----------------------- *\
	\* \/ Ship State Saving \/ */

public:
	/**
	 * Saves this ships state.
	 */
	UFUNCTION(BlueprintCallable)
	void SaveState();

	/**
	 * Loads this ships state.
	 */
	void LoadState(bool bLoadMold = false);

protected:
	/**
	 * Gets the save slot name used to save this ship
	 */
	virtual FString GetSaveStateSlotNamePrefix();

	/**
	 * Gets the save slot ID used to identify this when saving
	 */
	virtual FString GetSaveStateSlotID();

private:
	/**
	 * Gets the save slot name used when loading this ship.
	 */
	FString GetSaveStateSlotName();

	/* /\ Ship State Saving /\ *\
	\* ----------------------- */

};

/* /\ ==== /\ *\
|  /\ Ship /\  |
\* /\ ==== /\ */





/* \/ =============== \/ *\
|  \/ Ship Save State \/  |
\* \/ =============== \/ */

UCLASS()
class VOIDSINGER_API UShipSaveState : public USaveGame
{
	GENERATED_BODY()
	
public:
	//Stores the state of Voidgrid of the ship saved by this.
	FVoidgridState VoidgridState;

	//Stores the Location of the ship saved by this.
	FVector2D Location;
};

/* /\ =============== /\ *\
|  /\ Ship Save State /\  |
\* /\ =============== /\ */