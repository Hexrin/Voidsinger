// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Voidsongs/BaseVerbVoidsong.h"
#include "StarSystemLevelManager.generated.h"

/*
 * As far as I can tell this system doesnt support multiple simultaneous Voidsongs which we were planning to implement - Liam Suggestion
 */

//The below interface should be commented. What should implement it and what functionality does it support? - Liam Suggestion
UINTERFACE(Blueprintable)
class UActivateInterface : public UInterface
{
	GENERATED_BODY()
};

class IActivateInterface
{
	GENERATED_BODY()

public:
	/*
	* The below function should be commented.
	* What should the function do?
	* How should it be overriden?
	* - Liam Suggestion
	*/ 
	virtual void Activate(float Duration);
};

//The below delagates should be commented - Liam Suggestion
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FVoidsongDelegate, const TArray<TEnumAsByte<EFactions>>& , Factions, const TArray<TSubclassOf<UObject>>&, NounClasses, const TArray<UBaseVoidsong*>&, AvailableVoidsongs, float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeactivateVoidsongDelegate);

UCLASS()
class VOIDSINGER_API AStarSystemLevelManager : public ALevelManager
{
	GENERATED_BODY()

public:
	/*
	* The below function should be commented.
	* What does the function do?
	* What should be in each of the parameters? Maybe add UPARAM tooltips for this.
	* ActorThatActivated should be replaced with Instigator to maintain consitancy with ue5
	* 
	* Weird function name.
	* How is this differnt fromt he nonexistant function ActivateWithoutEffects
	* What is it activating?
	* - Liam Suggestion
	*/	
	UFUNCTION(BlueprintCallable)
	void ActivateWithEffects(AActor* ActorHit, AActor* ActorThatActivated, TArray<UBasePart*> PartsHit, FVector LocationCalledFrom, FVector WorldLocation, float Effectiveness, float Duration);

	/*
	* The below function should be commented. 
	* What does the function do?
	* What should be in each of the parameters? Maybe add UPARAM tooltips for this.
	* 
	* Function name confusing.
	* What is it Brocasting? 
	* It also shares a name with a delagate function making it hard to ctrl f.
	* - Liam Suggestion
	*/
	UFUNCTION(BlueprintCallable)
	void Broadcast(TArray<TEnumAsByte<EFactions>> Factions, TArray<TSubclassOf<UObject>> NounClasses, TArray<UBaseVerbVoidsong*> Verbs, TArray<UBaseVoidsong*> AvailableVoidsongs, float Duration);
	
	/*
	* The below function should be commented. What does the function do?
	* 
	* Maybe confusing name. 
	* What does it mean to set/unset a verb?
	* Maybe rename to DeactivateAllVerbs?
	* - Liam Suggestion
	*/ 
	UFUNCTION()
	void UnsetVerbs();

	/*
	* The below function should be commented.
	* What does the function do?
	* What is it returning and how should/could that return value be used
	* - Liam Suggestion
	*/
	UFUNCTION()
	FVoidsongDelegate GetVoidsongDelegate();

	/*
	* The below function should be commented.
	* What does the function do?
	* What is it returning and how should/could that return value be used
	* 
	* Function name is gramticaly weird. Consider renaming to GetActiveVerbs
	* - Liam Suggestion
	*/
	UFUNCTION(BlueprintPure)
	TArray<UBaseVerbVoidsong*> GetVerbsActive();

	/*
	* The below delagate should be commented. Details of when is it called? 
	* Should have a better name that describe when it is called.
	* - Liam Suggestion
	*/
	UPROPERTY(BlueprintAssignable)
	FVoidsongDelegate OnVoidsongDelegate;

	//The below delagate should be commented. Details of when is it called? - Liam Suggestion
	UPROPERTY(BlueprintAssignable)
	FDeactivateVoidsongDelegate OnDeactivateVoidsongDelegate;

	/*
	* The below varaible should be commented.
	* What is it tracking and how is it used?
	*
	* Variable name is gramticaly weird. Consider renaming to ActiveVerbs
	* - Liam Suggestion
	*/
	UPROPERTY()
	TArray<UBaseVerbVoidsong*> VerbsActive;

};
