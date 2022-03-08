// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsong.h"
#include "VoidsongData.h"
#include "VoidsongManager.generated.h"

/* \/ ================ \/ *\
|  \/ UVoidsongManager \/  |
\* \/ ================ \/ */

/* --------------- *\
\* \/ Delegates \/ */

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
	 * Plays a Voidsong with the given Voidsong data
	 *
	 * @param VoidsongData - The Voidsong data played
	 *
	 * @return - The Voidsong played
	 */
	UFUNCTION(BlueprintCallable, Category = "VoisdongActivation")
	UVoidsong* PlayVoidsong(FVoidsongData VoidsongData);

	//Delegate called when any Voidsong is played
	UPROPERTY(BlueprintAssignable)
	FVoidsongPlayed OnVoidsongPlayed;
	
	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};

/* /\ ================ /\ *\
|  /\ UVoidsongManager /\  |
\* /\ ================ /\ */