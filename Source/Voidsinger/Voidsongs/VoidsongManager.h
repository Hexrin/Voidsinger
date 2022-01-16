// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseMotif.h"
#include "Factions/BaseFactionMotif.h"
#include "Nouns/BaseNounMotif.h"
#include "Verbs/BaseVerbMotif.h"
#include "VoidsongManager.generated.h"

/* --------------- *\
\* \/ Delegates \/ */

//DECLARE_MULTICAST_DELEGATE_FiveParams(FVoidsongCue, const TArray<EFaction>&, Factions, const TArray<ENoun>&, Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>&, Verbs, const TArray<TSubclassOf<UBaseMotif>>&, PlayableMotifs, float, Effectiveness);

/* /\ Delegates /\ *\
\* --------------- */

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UVoidsongManager : public UObject
{
	GENERATED_BODY()

	//UPROPERTY(BlueprintAssignable)

	
};
