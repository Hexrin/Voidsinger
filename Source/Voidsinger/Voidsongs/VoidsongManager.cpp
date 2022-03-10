// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidsongManager.h"

/* ------------------------- *\
\* \/ Voidsong Activation \/ */

/**
* Plays a Voidsong with the given Voidsong data
*
* @param VoidsongData - The Voidsong data played
*
* @return The Voidsong played
*/
UVoidsong* UVoidsongManager::PlayVoidsong(FVoidsongData VoidsongData)
{
	//Create the new Voidsong object
	UVoidsong* NewVoidsong = NewObject<UVoidsong>(UVoidsong::StaticClass());
	NewVoidsong->InitializeVariables(VoidsongData);

	//Broadcast the OnVoidsongPlayed delegate
	OnVoidsongPlayed.Broadcast(NewVoidsong);

	return NewVoidsong;
}

/* /\ Voidsong Activation /\ *\
\* ------------------------- */