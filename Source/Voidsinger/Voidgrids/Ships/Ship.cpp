// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "Voidsinger/StarSystemGameMode.h"

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

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
 * Plays a Voidsong with the given Motifs
 *
 * @param Factions - The Faction Motifs played
 * @param Nouns - The Noun Motifs played
 * @param Verbs - The Verb Motifs played
 */
void AShip::PlayVoidsong(const TArray<TSubclassOf<UBaseFactionMotif>>& Factions, const TArray<TSubclassOf<UBaseNounMotif>>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs)
{
	//Call the globally available "PlayVoidsong" function on the Game Mode
	if (IsValid(Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))))
	{
		Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->VoidsongManager->PlayVoidsong(Factions, Nouns, Verbs, PlayableMotifs);
	}
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */