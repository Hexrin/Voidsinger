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
*
* @return - The Voidsong played
*/
UVoidsong* UVoidsongManager::PlayVoidsong(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs)
{
	//Create the new Voidsong object
	UVoidsong* NewVoidsong = NewObject<UVoidsong>(UVoidsong::StaticClass());
	NewVoidsong->InitializeVariables(Factions, Nouns, Verbs);

	// \/ Find Nouns and Factions played from the Motifs \/

	TArray<EFaction> FactionsPlayed;
	TArray<ENoun> NounsPlayed;

	for (UBaseFactionMotif* EachFactionMotif : Factions)
	{
		FactionsPlayed.Emplace(Cast<UBaseFactionMotif>(EachFactionMotif)->Faction);
	}
	for (UBaseNounMotif* EachNounMotif : Nouns)
	{
		NounsPlayed.Emplace(Cast<UBaseNounMotif>(EachNounMotif)->Noun);
	}

	// /\ Find Nouns and Factions played from the Motifs /\

	//Broadcast the OnVoidsongPlayed delegate
	OnVoidsongPlayed.Broadcast(NewVoidsong);

	return NewVoidsong;

}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */