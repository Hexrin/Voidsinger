// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"

/* ---------------------------------- *\
\* \/ Playable Voidsong Management \/ */

/**
 * Adds Voidsongs to the list of Voidsongs that this component can play.
 *
 * @param VoidsongsAdded - The new playable Voidsongs
 */
void AShip::AddNewVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> VoidsongsAdded)
{
	for (TSubclassOf<UBaseVoidsong> EachVoidsongAdded : VoidsongsAdded)
	{
		PlayableVoidsongs.Emplace(EachVoidsongAdded);
	}
}

/* /\ Playable Voidsong Management /\ *\
\* ---------------------------------- */

void AShip::PlayVoidsongs(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs)
{
}
