// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BaseVoidsong.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseVoidsong : public UObject
{
	GENERATED_BODY()
	
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             FUNCTIONS             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

public:

	/*Initializer Functions*\
	\*--------------------*/

	//Constructor
	UBaseVoidsong();

	//GetWorld() Override
	virtual class UWorld* GetWorld() const override;

	/*--------Tick--------*\
	\*--------------------*/

	//Called every frame
	virtual void Tick(float DeltaTime);

	//Determines if this part will tick
	virtual bool IsTickable() const;

	//Is a nesseary part of tick component
	virtual TStatId GetStatId() const;

public:
	//Event Tick for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void EventTick(float DeltaTime);

	/*Voidsong Functions*\
	\*------------------*/

	//Used to activate the voidsong.
	UFUNCTION(BlueprintCallable)
	void Activate();

	//This will do the effect of the voidsong. It's called within Activate. Needs to be implemented for each Voidsong.
	UFUNCTION(BlueprintImplementableEvent)
	void Effect();

	//This will undo the effect of the voidsong. It's also called within activate, after a delay for the duration. Needs to be implemented for each Voidsong. 
	UFUNCTION(BlueprintImplementableEvent)
	void Deactivate();

	//This function is for the delay of the duration.
	UFUNCTION()
	void DurationDelay();

	//This function is for the delay of the cooldown.
	UFUNCTION()
	void CooldownDelay();

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Blueprint Defaults*\
	\*------------------*/

	//The duration of the voidsong.
	float Duration;

	//The cooldown of the voidsong.
	float Cooldown;

	//The activation key combo of the voidsong.
	TArray<int> ActivationCombo;

	UPROPERTY()
	bool bIsBeingDestroyed;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EVoidsongs> VoidsongType;

	/*Instanced  Variables*\
	\*--------------------*/

private:
	//Used to check if the voidsong is off cooldown.
	UPROPERTY()
	bool CanActivateAgain;
};