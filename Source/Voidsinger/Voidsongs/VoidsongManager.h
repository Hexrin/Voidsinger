// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsong.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BaseMotif.h"
#include "Factions/BaseFactionMotif.h"
#include "Nouns/BaseNounMotif.h"
#include "Verbs/BaseVerbMotif.h"
#include "VoidsongManager.generated.h"

/* --------------- *\
\* \/ Delegates \/ */

/**
 * Delegate called when any Voidsong is played
 * 
 * @param Voidsong - The Voidsong object of this Voidsong
 * @param Factions - The Factions played
 * @param Nouns - The Nouns played
 * @param Verbs - The Verbs played
 * @param PlayableMotifs - The Motifs playable by whatever played this Voidsong
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FVoidsongPlayed, UVoidsong*, Voidsong, const TArray<EFaction>&, Factions, const TArray<ENoun>&, Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>&, Verbs, const TSet<TSubclassOf<UBaseMotif>>&, PlayableMotifs);

/* /\ Delegates /\ *\
\* --------------- */

/**
 * Class for managing playing Voidsongs
 */
UCLASS()
class VOIDSINGER_API UVoidsongManager : public UObject
{
	GENERATED_BODY()

	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

public:

	/**
	* Plays a Voidsong with the given Motifs
	*
	* @param Factions - The Faction Motifs played
	* @param Nouns - The Noun Motifs played
	* @param Verbs - The Verb Motifs played
	* @param PlayableMotifs - The Motifs playable by whatever played this Voidsong
	*/
	UFUNCTION(BlueprintCallable, Category = "VoisdongActivation")
	void PlayVoidsong(const TArray<TSubclassOf<UBaseFactionMotif>>& Factions, const TArray<TSubclassOf<UBaseNounMotif>>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs, const TSet<TSubclassOf<UBaseMotif>>& PlayableMotifs);

	//Delegate called when any Voidsong is played
	UPROPERTY(BlueprintAssignable)
	FVoidsongPlayed OnVoidsongPlayed;
	
	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
