// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BaseMotif.generated.h"

/*
* This class should not be able to be inscaniated as it has no values that change during runtime
* Aka it should be a static class.
* The only way to create a static class that can be overriden in BP is a blueprint function library, so
* this class should derive from UBlueprintFunctionLibrary and all functions should be made static.
* All variables should be converted to non-UPROPERTY() public static const varaibles.
* Sadly this means that they will not be able to be set in blueprints but we should be using config files anyways.
* 
* If this is done the class should no longer derive from FTickableGameObject.
*/

/**
 * The base Voidsong is the base class for all Voidsongs. It holds information such as the activation sequence for the Voidsong.
 */
UCLASS(Abstract, Blueprintable, config=VoidSettings, defaultconfig)
class VOIDSINGER_API UBaseMotif : public UDataAsset
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Sets default values for this object's properties
	 */
	UBaseMotif();

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ------------------- *\
	\* \/ Voidsong Data \/ */

public:

	//Stores a sequence of numbers that will activate the Voidsong when played.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, config, meta = (Category = "Config|VoidsongData"))
	TArray<int32> ActivationSequence;
	
	//Stores the amount of time that this Voidsong plays for
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, config, meta = (Category = "Config|VoidsongData"))
	float Duration;

	/* /\ VoidsongData /\ *\
	\* ------------------ */
	
};
