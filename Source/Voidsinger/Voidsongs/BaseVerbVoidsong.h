// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/Voidsongs/BaseNounVoidsong.h"
#include "Voidsinger/Voidsongs/BaseWhoVoidsong.h"
#include "BaseVerbVoidsong.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseVerbVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()
	
public:

	/*Voidsong Functions*\
	\*------------------*/

	//Used to activate the voidsong.
	UFUNCTION(BlueprintCallable)
	void Activate(TArray<UBaseWhoVoidsong*> Whos, TArray<UBaseNounVoidsong*> Nouns);

	//This will do the effect of the voidsong. It's called within Activate. Needs to be implemented for each Voidsong.
	UFUNCTION(BlueprintImplementableEvent)
	void Effect(const TArray<UBaseWhoVoidsong*>& Whos, const TArray<UBaseNounVoidsong*>& Nouns);

	//This will undo the effect of the voidsong. It's also called within activate, after a delay for the duration. Needs to be implemented for each Voidsong. 
	UFUNCTION(BlueprintImplementableEvent)
	void Deactivate();

	//This function is for the delay of the duration.
	UFUNCTION()
	void DurationDelay();

	/*Getters*\
	\--------*/

private:

	/*Instanced  Variables*\
	\*--------------------*/

	//The duration of the voidsong.
	UPROPERTY()
	float Duration;

	//Used to check if the voidsong is off cooldown.
	UPROPERTY()
	bool CanActivateAgain = true;

	UPROPERTY()
	TArray<UBaseWhoVoidsong*> WhosAffected;

	UPROPERTY()
	TArray<UBaseNounVoidsong*> NounsAffected;

};
