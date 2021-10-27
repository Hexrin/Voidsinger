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

	//Pre-activate calls activate, but it also sets ActorHit and WorldLocation. It's convenient.
	UFUNCTION()
	void PreActivate(AActor* NewActorHit, FVector NewWorldLocation);

	//This will do the effect of the voidsong. It's called within Activate. Needs to be implemented for each Voidsong.
	UFUNCTION(BlueprintImplementableEvent)
	void Activate(AActor* NewActorHit, FVector NewWorldLocation);

	//This will undo the effect of the voidsong. It's also called within activate, after a delay for the duration. Needs to be implemented for each Voidsong. 
	UFUNCTION(BlueprintImplementableEvent)
	void Deactivate();

	/*Getters*\
	\--------*/

protected:

	/*Instanced  Variables*\
	\*--------------------*/

	UPROPERTY(BlueprintReadOnly)
	AActor* ActorHit;

	UPROPERTY(BlueprintReadOnly)
	FVector WorldLocation;

};
