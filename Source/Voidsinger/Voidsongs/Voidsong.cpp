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
 */
void UVoidsong::InitializeVariables(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs)
{
	// \/ Set Variables \/
	FactionMotifs = Factions;
	NounMotifs = Nouns;
	VerbMotifs = Verbs;

	// /\ Set Variables /\

	// \/ Find Duration \/

	for (UBaseFactionMotif* EveryFactionMotif : FactionMotifs)
	{
		Duration += EveryFactionMotif->Duration;
	}
	for (UBaseNounMotif* EveryNounMotif : NounMotifs)
	{
		Duration += EveryNounMotif->Duration;
	}
	for (UBaseVerbMotif* EveryVerbMotif : VerbMotifs)
	{
		Duration += EveryVerbMotif->Duration;
	}

	// /\ Find Duration /\

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
	//Broadcast the ForDuration delegate
	ForDuration.Broadcast(DeltaTime);

	// \/ Check if the Voidsong has played for the correct Duration, if so, Deactivate \/

	TimeSincePlayed += DeltaTime;

	if (TimeSincePlayed >= Duration)
	{
		Deconstruct();
	}

	// /\ Check if the Voidsong has played for the correct Duration, if so, Deactivate /\

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
void UVoidsong::Deconstruct()
{
	//Unbind the delegates
	ForDuration.Clear();
	OnBeat.Clear();
	
	//Destroy the object
	ConditionalBeginDestroy();
}

/* /\ Deactivation /\ *\
\* ------------------ */