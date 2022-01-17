// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BaseMotif.generated.h"

/**
 * The base Voidsong is the base class for all Voidsongs. It holds information such as the activation sequence for the Voidsong.
 */
UCLASS(Abstract, Blueprintable)
class VOIDSINGER_API UBaseMotif : public UDataAsset
{
	GENERATED_BODY()

	/* ------------------- *\
	\* \/ Voidsong Data \/ */

public:

	//Stores a sequence of numbers that will activate the Voidsong when played.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (Category = "VoidsongData"))
	TArray<int32> ActivationSequence;
	
	//Stores the amount of time that this Voidsong plays for
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (Category = "VoidsongData"))
	float Duration;

	/* /\ VoidsongData /\ *\
	\* ------------------ */
	
};
