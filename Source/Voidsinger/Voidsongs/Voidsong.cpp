// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsong.h"

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes the variables of this object
 *
 * @param Factions - The Factions played in this Voidsong
 * @param Nouns - The Nouns played in this Voidsong
 * @param Verbs - The Verbs played in this Voidsong
 * @param AllPlayableMotifs - The motifs that are playable by whatever played the Voidsong
 */
void UVoidsong::InitializeVariables(const TArray<TSubclassOf<UBaseFactionMotif>>& Factions, const TArray<TSubclassOf<UBaseNounMotif>>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs, const TSet<TSubclassOf<UBaseMotif>>& AllPlayableMotifs)
{
	// \/ Set Variables \/
	FactionMotifs = Factions;
	NounMotifs = Nouns;
	VerbMotifs = Verbs;
	PlayableMotifs = AllPlayableMotifs;

	// /\ Set Variables /\

	// \/ Find Duration \/

	for (TSubclassOf<UBaseFactionMotif> EveryFactionMotif : FactionMotifs)
	{
		Duration += EveryFactionMotif.GetDefaultObject()->Duration;
	}
	for (TSubclassOf<UBaseNounMotif> EveryNounMotif : NounMotifs)
	{
		Duration += EveryNounMotif.GetDefaultObject()->Duration;
	}
	for (TSubclassOf<UBaseVerbMotif> EveryVerbMotif : VerbMotifs)
	{
		Duration += EveryVerbMotif.GetDefaultObject()->Duration;
	}

	// /\ Find Duration /\

	bIsActive = true;
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ---------- *\
\* \/ Tick \/ */

/**
 * Called every tick
 *
 * @param DeltaTime - The time since the previous tick
 */
void UVoidsong::Tick(float DeltaTime)
{
	if (bIsActive)
	{
		//Broadcast the ForDuration delegate
		ForDuration.Broadcast(DeltaTime);

		// \/ Check if the Voidsong has played for the correct Duration, if so, Deactivate \/

		TimePlayed += DeltaTime;

		if (TimePlayed >= Duration)
		{
			Deactivate();
		}

		// /\ Check if the Voidsong has played for the correct Duration, if so, Deactivate /\

	}
}

/**
 * Determines if this object is tickable
 *
 * @return - Whether this object is tickable or not
 */
bool UVoidsong::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject));
}

/**
 * Returns this object's StatId
 *
 * @return - This object's StatId
 */
TStatId UVoidsong::GetStatId() const
{
	return TStatId();
}

/* /\ Tick /\ *\
\* ---------- */

/* ------------------ *\
\* \/ Deactivation \/ */

/**
 * Deactivates the Voidsong
 */
void UVoidsong::Deactivate()
{
	bIsActive = false;
}

/* /\ Deactivation /\ *\
\* ------------------ */