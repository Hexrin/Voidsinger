// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseVoidsong.h"
#include "Factions/BaseFactionVoidsong.h"
#include "Nouns/BaseNounVoidsong.h"
#include "Verbs/BaseVerbVoidsong.h"
#include "VoidsongComponent.generated.h"

//Old delegates from AShip (put here for reference, will be deleted)
//Delegates should be commented. What functions should they be bound to? - Liam 
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddVoidsongDelegate, UBaseVoidsong*, AddedVoidsong);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FActivatePartsDelegate, const TArray<EFaction>&, Factions, const TArray<TSubclassOf<UObject>>&, NounClasses, const TArray<UBaseVoidsong*>&, AvailableVoidsongs);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnsetVerbsDelegate);

/**
 * The VoidsongComponent handles what Voidsongs are available to play, Voidsong activation and deactivation event handlers, and deciding what Voidsongs are played given a sequence of integers.
 * It also keeps track of what Voidsongs are active on this component.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIDSINGER_API UVoidsongComponent : public UActorComponent
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:	

	/**
	 * Sets default values for this component's properties
	 */
	UVoidsongComponent();

	/**
	 * Called every frame
	 * 
	 * @param DeltaTime - Time since the previous tick
	 * @param TickType - The tick type
	 * @param ThisTickFunction - I have no idea
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	/**
	 * Called when the game starts
	 */
	virtual void BeginPlay() override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ------------------------------------------------------------------------------------- *\
	\* \/ Old Voidsong functions from AShip (put here for reference, will be deleted) \/ */

	//Adds a new Voidsong to the AvailableVoidsongs. Will be useful for gaining a new voidsong.
	//UFUNCTION(BlueprintCallable)
		//void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	//UE doesnt like int, use int32 or int64 instead. - Liam Suggestion	
	//Plays a voidsong with a given activation sequence
	//UFUNCTION()
		//void PlaySequence(TArray<int> Sequence);

	/*
	* UE doesnt like int, use int32 or int64 instead.
	*
	* Confusing Fucntion Name.
	* What does it mean to decide a voidsong?
	* What is the return value?
	*
	* Confusing Comment.
	* Comment is vague. What does "figues out" and "played" mean?
	* What is this function returning?
	* What are each of the parameters used for/ what is supposed to be input?
	* - Liam Suggestion
	*/
	//Figures out what Voidsongs are played.
	//UFUNCTION()
		//float DecideVoidsongsPlayed(TArray<int> Sequence, TArray<EFaction>& Factions, TArray<TSubclassOf<UObject>>& Nouns, TArray<UBaseVerbVoidsong*>& Verbs);

	/*
	* Confusing Fucntion Name.
	* What is a duration delay?
	* Judgeing by the name I'd think this function would return the duration of a delay for a voidsong.
	* Consider Renaming to EndVoidsong() or similar
	*
	* Confusing Comment.
	* The domment should not say when the function is used but what it does.
	* - Liam Suggestion
	*/
	//This function is for the delay of the Voidsong duration
	//UFUNCTION()
		//void DurationDelay();

	//Consider swapping the TArray for a TSet as duplicate Voidsongs should not be possible - Liam Suggestion
	//Creates Voidsong objects with the given classes of Voidsongs and adds them to the AvaialableVoidsongs. Will be useful for loading from a save game.
	//UFUNCTION(BlueprintCallable)
		//void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	/*
	* Noun classes dosnt make sense in this context.
	* Either rename the parameter to PartClasses or,
	* rename the function to something realted to Noun Voidsongs, maybe BrodcastPlayNouns.
	*
	* Souldn't NounClasses be a TArray<TSubclassOf<UBasePart>>&? Especially if the function is called activate *parts*.
	* - Liam Suggestion
	*/
	//Event dispatcher for activating the parts on the ship.
	//UFUNCTION(BlueprintCallable)
	//void BroadcastActivateParts(const TArray<TSubclassOf<UObject>>& NounClasses);
		
	/* /\ Old Voidsong functions from AShip (put here for reference, will be deleted) /\ *\
	\* ------------------------------------------------------------------------------------- */

	/* ------------------------------------------------------------------------------------- *\
	\* \/ Old Voidsong variables from AShip (put here for reference, will be deleted) \/ */

	/*
	* Delegate needs comment.
	* Reader needs to know what it stores.
	* Since it stores a delegate the reader should know when the delagate is broadcast.
	* - Liam Suggestion
	*/
	//UPROPERTY(BlueprintAssignable)
	//FUnsetVerbsDelegate OnUnsetVerbsDelegate;

	/*
	* Comment phrasing wierd
	* Consider "Stores the voidsongs that this ship can play" or something similar.
	*
	* If suggestion on lines 11-18 of BaseVoidsong.h are followed then
	* this should be TSet<TSubclassOf<UBaseVoidsong>>
	* - Liam Suggestion
	*/
	//Array of the Voidsongs that are available to play
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<UBaseVoidsong*> AvailableVoidsongs;

	/*
	* Comment unclear
	* Reader does not need to know what the functiosn bound to this do but what it stores.
	* Since it stores a delegate the reader should know when the delagate is broadcast
	*
	* Why does this delegate exist?
	* It is only called on BroadcastActivateParts() which itself is never called
	* - Liam Suggestion
	*/
	//For firing parts on the ship
	//UPROPERTY(BlueprintAssignable)
	//FActivatePartsDelegate OnActivatePartsDelegate;

	/* /\ Old Voidsong variables from AShip (put here for reference, will be deleted) /\ *\
	\* ------------------------------------------------------------------------------------- */

	/* ---------------------------------- *\
	\* \/ Playable Voidsong Management \/ */

public:

	/**
	 * Adds Voidsongs to the list of Voidsongs that this component can play.
	 * 
	 * @param VoidsongsAdded - The new playable Voidsongs
	 */
	UFUNCTION(BlueprintCallable, Category = "PlayableVoidsongManagement")
	void AddNewVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> VoidsongsAdded);

private:

	//Stores the Voidsongs that this component can play.
	TSet<TSubclassOf<UBaseVoidsong>> PlayableVoidsongs;

	/* /\ Playable Voidsong Management /\ *\
	\* ---------------------------------- */

	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

private:

	/**
	 * 
	 */
	void PlaySequence(TArray<int32> Sequence);

	/**
	 *
	 */
	float ParseSequenceIntoVoidsongData(TArray<int32> Sequence, TArray<EFaction>& OutFactions, TArray<ENoun>& OutNouns, TArray<TSubclassOf<UBaseVerbVoidsong>>& OutVerbs);

	/**
	 *
	 */
	void EndSequence();

	//
	bool bCanPlaySequences;

	//
	TArray<TSubclassOf<UBaseVoidsong>> ActiveVoidsongs;

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */


};
