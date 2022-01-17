// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "VoidsongData.h"
#include "Voidsong.generated.h"


/* --------------- *\
\* \/ Delegates \/ */

/**
 * Delegate for calling Voidsong Cues
 * 
 * @param Verbs - The Verbs played in this Voidsong
 * @param Effectiveness - The effectiveness of the activation, useful when called every tick
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVoidsongCue, const TArray<UBaseVerbMotif*>, Verbs, float, Effectiveness);

/* /\ Delegates /\ *\
\* --------------- */

/**
 * Class for broadcasting delegates related to a specific Voidsong played
 */
UCLASS()
class VOIDSINGER_API UVoidsong : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:


	/**
	 * Initializes the variables of this Voidsong
	 *
	 * @param Data - The Voidsong data of this Voidsong
	 */
	void InitializeVariables(FVoidsongData Data);

	//The data of this Voidsong
	FVoidsongData VoidsongData;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------- *\
	\* \/ Tick \/ */

private:

	/**
	 * Called every tick
	 * 
	 * @param DeltaTime - The time since the previous tick
	 */
	virtual void Tick(float DeltaTime) override;

	/** 
	 * Determines if this object is tickable
	 * 
	 * @return - Whether this object is tickable or not
	 */
	virtual bool IsTickable() const override;

	/**
	 * Returns this object's StatId
	 * 
	 * @return - This object's StatId
	 */
	virtual TStatId GetStatId() const override;

	/* /\ Tick /\ *\
	\* ---------- */
	
	/* ------------------- *\
	\* \/ Voidsong Cues \/ */

public: 

	//Called every tick the Voidsong is active
	UPROPERTY(BlueprintAssignable)
	FVoidsongCue ForDuration;

	//Called every beat of the Voidsong
	UPROPERTY(BlueprintAssignable)
	FVoidsongCue OnBeat;

	/* /\ Voidsong Cues /\ *\
	\* ------------------- */

	/* ------------------ *\
	\* \/ Deactivation \/ */

private:

	/**
	 * Deconstructs the Voidsong
	 */
	void Deconstruct();

	//Time since the Voidsong was activated
	float TimeSincePlayed;

	/* /\ Deactivation /\ *\
	\* ------------------ */
};
