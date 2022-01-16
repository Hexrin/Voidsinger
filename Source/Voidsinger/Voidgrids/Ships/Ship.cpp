// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"

/* ---------------------------------- *\
\* \/ Playable Voidsong Management \/ */

/**
 * Adds Voidsongs to the list of Voidsongs that this component can play.
 *
 * @param VoidsongsAdded - The new playable Voidsongs
 */
void AShip::AddNewMotifs(TArray<TSubclassOf<UBaseMotif>> MotifsAdded)
{
	for (TSubclassOf<UBaseMotif> EachMotifAdded : MotifsAdded)
	{
		PlayableMotifs.Emplace(EachMotifAdded);
	}
}

/* /\ Playable Voidsong Management /\ *\
\* ---------------------------------- */

void AShip::PlayVoidsong(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs)
{
}
