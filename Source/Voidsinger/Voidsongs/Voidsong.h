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

//Comment - Liam Suggestion
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

	//Why is PlayableMotifs a parameter? The voidsongs should not care what motifs a thing can play. The voidsong player should be responsable for whether or not it can play a song. - Liam Suggestion
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
	//Why does this exist? Shouldn't this object be destroyed as soon as it is finshed playing and not exist before it starts? - Liam Suggestion
	//Tracks whether this Voidsong is actively playing
	bool bIsActive;

	//The length of the Voidsong
	float Duration;

	//Is nesseary to divide motifs by type ? If it truly is then consider making a struct for redablityand easy iteration over all motifs. - Liam Suggestion

	//Don't use SubclassOf for data assets just use UBaseMotif* - Liam Suggestion
	//The Faction Motifs played in this Voidsong
	TArray<TSubclassOf<UBaseFactionMotif>> FactionMotifs;

	//Don't use SubclassOf for data assets just use UBaseMotif* - Liam Suggestion
	//The Noun Motifs played in this Voidsong
	TArray<TSubclassOf<UBaseNounMotif>> NounMotifs;

	//Don't use SubclassOf for data assets just use UBaseMotif* - Liam Suggestion
	//The Verb Motifs played in this Voidsong
	TArray<TSubclassOf<UBaseVerbMotif>> VerbMotifs;

	//Don't use SubclassOf for data assets just use UBaseMotif* - Liam Suggestion
	//Why is PlayableMotifs a parameter? The voidsongs should not care what motifs a thing can play. The voidsong player should be responsable for whether or not it can play a song. - Liam Suggestion
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
	//Why is this needed? I dont think voidsongs will ever be paused while the rest of the game isn't - Liam Suggestion
	/**
	 * Deactivates the Voidsong
	 */
	void Deactivate();

	//Shouldn't this be called TimeSincePlayed then? - Liam Suggestion
	//Time since the Voidsong was activated
	float TimePlayed;

	/* /\ Deactivation /\ *\
	\* ------------------ */
};
