// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseShip.h"

/* ---------------------------------- *\
\* \/ Playable Voidsong Management \/ */

/**
 * Adds Voidsongs to the list of Voidsongs that this component can play.
 *
 * @param VoidsongsAdded - The new playable Voidsongs
 */
void ABaseShip::AddNewVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> VoidsongsAdded)
{
	for (TSubclassOf<UBaseVoidsong> EachVoidsongAdded : VoidsongsAdded)
	{
		PlayableVoidsongs.Emplace(EachVoidsongAdded);
	}
}

/* /\ Playable Voidsong Management /\ *\
\* ---------------------------------- */

void ABaseShip::BroadcastPlayVoidsongs(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs)
{
}
