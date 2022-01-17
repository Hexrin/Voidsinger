// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Factions/BaseFactionMotif.h"
#include "Nouns/BaseNounMotif.h"
#include "Verbs/BaseVerbMotif.h"
#include "Voidsong.generated.h"


/* --------------- *\
\* \/ Delegates \/ */

/**
 * Delegate for calling Voidsong Cues
 * 
 * @param Effectiveness - The effectiveness of the activation, useful when called every tick
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidsongCue, float, Effectiveness);

/* /\ Delegates /\ *\
\* --------------- */

/**
 * Liam told me to type with my thumbs for 1 comment
 */
UCLASS()
class VOIDSINGER_API UVoidsong : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Initializes the variables of this object
	 *
	 * @param Factions - The Factions played in this Voidsong
	 * @param Nouns - The Nouns played in this Voidsong
	 * @param Verbs - The Verbs played in this Voidsong
	 * @param AllPlayableMotifs - The motifs that are playable by whatever played the Voidsong
	 */
	void InitializeVariables(const TArray<TSubclassOf<UBaseFactionMotif>>& Factions, const TArray<TSubclassOf<UBaseNounMotif>>& Nouns, const TArray<TSubclassOf<UBaseVerbMotif>>& Verbs, const TSet<TSubclassOf<UBaseMotif>>& AllPlayableMotifs);

private:

	//Tracks whether this Voidsong is actively playing
	bool bIsActive;

	//The length of the Voidsong
	float Duration;

	//The Faction Motifs played in this Voidsong
	TArray<TSubclassOf<UBaseFactionMotif>> FactionMotifs;

	//The Noun Motifs played in this Voidsong
	TArray<TSubclassOf<UBaseNounMotif>> NounMotifs;

	//The Verb Motifs played in this Voidsong
	TArray<TSubclassOf<UBaseVerbMotif>> VerbMotifs;

	//The Motifs that are playable by whatever played the Voidsong
	TSet<TSubclassOf<UBaseMotif>> PlayableMotifs;

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
	 * Deactivates the Voidsong
	 */
	void Deactivate();

	//Time since the Voidsong was activated
	float TimePlayed;

	/* /\ Deactivation /\ *\
	\* ------------------ */
};
