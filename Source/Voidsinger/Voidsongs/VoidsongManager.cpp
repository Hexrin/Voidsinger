// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidsongManager.h"

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
* Plays a Voidsong with the given Motifs
*
* @param Factions - The Faction Motifs played
* @param Nouns - The Noun Motifs played
* @param Verbs - The Verb Motifs played
* @param PlayableMotifs - The Motifs playable by whatever played this Voidsong
*/
void UVoidsongManager::PlayVoidsong(const TArray<TSubclassOf<UBaseFactionMotif>>& Factions, const TArray<TSubclassOf<UBaseNounMotif>>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs, const TSet<TSubclassOf<UBaseMotif>>& PlayableMotifs)
{
	//Create the new Voidsong object
	UVoidsong* NewVoidsong = NewObject<UVoidsong>(UVoidsong::StaticClass());
	NewVoidsong->InitializeVariables(Factions, Nouns, Verbs, PlayableMotifs);

	// \/ Find Nouns and Factions played from the Motifs \/

	TArray<EFaction> FactionsPlayed;
	TArray<ENoun> NounsPlayed;

	for (TSubclassOf<UBaseFactionMotif> EachFactionMotif : Factions)
	{
		FactionsPlayed.Emplace(Cast<UBaseFactionMotif>(EachFactionMotif->GetDefaultObject())->Faction);
	}
	for (TSubclassOf<UBaseNounMotif> EachNounMotif : Nouns)
	{
		NounsPlayed.Emplace(Cast<UBaseNounMotif>(EachNounMotif->GetDefaultObject())->Noun);
	}

	// /\ Find Nouns and Factions played from the Motifs /\

	//Broadcast the OnVoidsongPlayed delegate
	OnVoidsongPlayed.Broadcast(NewObject<UVoidsong>(UVoidsong::StaticClass()), FactionsPlayed, NounsPlayed, Verbs, PlayableMotifs);
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */