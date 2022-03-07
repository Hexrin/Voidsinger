// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsong.h"

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes the variables of this Voidsong
 *
 * @param Data - The Voidsong data of this Voidsong
 */
void UVoidsong::InitializeVariables(FVoidsongData Data)
{
	if (!bIsInitialized)
	{
		VoidsongData = Data;
		bIsInitialized = true;
	}
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
	ForDuration.Broadcast(VoidsongData.GetVerbMotifs(), DeltaTime);

	// \/ Check if the Voidsong has played for the correct Duration, if so, Deactivate \/ /

	TimeSincePlayed += DeltaTime;

	if (TimeSincePlayed >= VoidsongData.GetDuration())
	{
		Deconstruct();
	}

	// /\ Check if the Voidsong has played for the correct Duration, if so, Deactivate /\ /

}

/**
 * Determines if this object is tickable
 *
 * @return Whether this object is tickable or not
 */
bool UVoidsong::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject));
}

/**
 * Returns this object's StatId
 *
 * @return This object's StatId
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

/* ------------- *\
\* \/ Getters \/ */

/**
 * Returns a copy of the Voidsong data
 *
 * @return Copy of VoidsongData
 */
FVoidsongData UVoidsong::GetVoidsongData()
{
	return FVoidsongData(VoidsongData.GetFactionMotifs(), VoidsongData.GetNounMotifs(), VoidsongData.GetVerbMotifs());
}

/* /\ Getters /\ *\
\* ------------- */