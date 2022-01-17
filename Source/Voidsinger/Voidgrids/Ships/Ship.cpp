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
void AShip::AddNewMotifs(TArray<UBaseMotif*> MotifsAdded)
{
	for (UBaseMotif* EachMotifAdded : MotifsAdded)
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
void AShip::PlayVoidsong(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs)
{

	// \/ Check if Factions is empty, if so play every playable Faction \/

	TArray<UBaseFactionMotif*> FactionsToPlay;
	
	if (!Factions.IsEmpty())
	{
		FactionsToPlay = Factions;
	}
	else
	{
		for (UBaseMotif* EachPlayableMotif : PlayableMotifs)
		{
			if (IsValid(Cast<UBaseFactionMotif>(EachPlayableMotif)))
			{
				FactionsToPlay.Emplace(Cast<UBaseFactionMotif>(EachPlayableMotif));
			}
		}
	}

	// /\ Check if Factions is empty, if so play every playable Faction /\

	// \/ Check if Nouns is empty, if so play every playable Noun \/

	TArray<UBaseNounMotif*> NounsToPlay;

	if (!Nouns.IsEmpty())
	{
		NounsToPlay = Nouns;
	}
	else
	{
		for (UBaseMotif* EachPlayableMotif : PlayableMotifs)
		{
			if (IsValid(Cast<UBaseNounMotif>(EachPlayableMotif)))
			{
				NounsToPlay.Emplace(Cast<UBaseNounMotif>(EachPlayableMotif));
			}
		}
	}

	// /\ Check if Nouns is empty, if so play every playable Noun /\
	
	//Call the globally available "PlayVoidsong" function on the Game Mode
	if (IsValid(Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))))
	{
		Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->VoidsongManager->PlayVoidsong(FactionsToPlay, NounsToPlay, Verbs);
	}
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */