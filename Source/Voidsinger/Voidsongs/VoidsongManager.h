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
* Is nesseary to divide motifs by type? If it truly is then consider making a struct for redablity and easy iteration over all motifs.
* - Liam Suggestion
*/
/**
 * Broadcasts a played Voidsong
 * 
 * @param Voidsong - The Voidsong object of this Voidsong
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidsongPlayed, UVoidsong*, Voidsong);

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
	* 
	* @return - The Voidsong played
	*/
	UFUNCTION(BlueprintCallable, Category = "VoisdongActivation")
	UVoidsong* PlayVoidsong(const TArray<UBaseFactionMotif*>& Factions, const TArray<UBaseNounMotif*>& Nouns, const TArray<UBaseVerbMotif*>& Verbs);

	//Delegate called when any Voidsong is played
	UPROPERTY(BlueprintAssignable)
	FVoidsongPlayed OnVoidsongPlayed;
	
	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};
