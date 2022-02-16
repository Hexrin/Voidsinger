// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "GameFramework/SaveGame.h"
#include "Voidsinger/Voidsongs/VoidsongData.h"
#include "Ship.generated.h"

/* \/ ==== \/ *\
|  \/ Ship \/  |
\* \/ ==== \/ */

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
	 * Adds Motifs to the list of Motifs that this ship can play.
	 *
	 * @param MotifsAdded - The new playable Motifs
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayableVoidsongManagement")
	void AddNewMotifs(TArray<UBaseMotif*> MotifsAdded);

protected:

	//Stores the Voidsongs that this ship can play.
	FVoidsongData PlayableMotifs;

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
	 * 
	 * @return - The Voidsong object that was played
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongActivation")
	UVoidsong* PlayVoidsong(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs);

	/*
	 * Deactivates this ship's ability to play Voidsongs
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongActivation")
	void DeactivatePlayingVoidsongs();

	/**
	 * Activates this ship's ability to play Voidsongs
	 */
	UFUNCTION(BlueprintCallable, Category = "VoidsongActivation")
	void ActivatePlayingVoidsongs();

protected:

	//Tracks whether this ship can play Voidsongs
	bool bCanPlayVoidsongs;

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