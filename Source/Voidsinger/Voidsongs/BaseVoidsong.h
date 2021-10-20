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

USTRUCT(BlueprintType, Blueprintable)
struct FVoidsongInputs
{
	GENERATED_BODY()
	
	//The inputs
	UPROPERTY(EditDefaultsOnly)
	int Input1;

	UPROPERTY(EditDefaultsOnly)
	int Input2;

	UPROPERTY(EditDefaultsOnly)
	int Input3;

	static const int NumOfInputs = 3;

	FVoidsongInputs()
	{
		FVoidsongInputs(0, 0, 0);
	}

	FVoidsongInputs(int New1, int New2, int New3)
	{
		Input1 = New1;
		Input2 = New2;
		Input3 = New3;
	}

	int GetNumOfInputs()
	{
		return NumOfInputs;
	}
	TArray<int> GetInputsAsArray()
	{
		TArray<int> OutArray;
		OutArray.Emplace(Input1);
		OutArray.Emplace(Input2);
		OutArray.Emplace(Input3);
		return OutArray;
	}

};

UCLASS(BlueprintType, Blueprintable)
class VOIDSINGER_API UBaseVoidsong : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             FUNCTIONS             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

public:

	/*Initializer Functions*\
	\*---------------------*/

	//Constructor
	UBaseVoidsong();

	//GetWorld() functionality
	virtual class UWorld* GetWorld() const override;

	/*--------Tick--------*\
	\*--------------------*/

	//Called every frame
	virtual void Tick(float DeltaTime) override;

	//Determines if this part will tick
	virtual bool IsTickable() const override;

	//Is a nesseary part of tick component
	virtual TStatId GetStatId() const override;

public:
	//Event Tick for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void EventTick(float DeltaTime);

	/*Getters*\
	\*-------*/

	//Gets the activation combo of this voidsong
	UFUNCTION(BlueprintPure)
	TArray<int> GetActivationCombo();

	//Gets the display text of the Voidsong
	UFUNCTION(BlueprintPure)
	FText GetVoidsongDisplayText();

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
	UPROPERTY(EditDefaultsOnly)
	float Duration;

	//The cooldown of the voidsong.
	UPROPERTY(EditDefaultsOnly)
	float Cooldown;

	//The name of the Voidsong that the player will see.
	UPROPERTY(EditDefaultsOnly)
	FText VoidsongDisplayText;

	/*Instanced  Variables*\
	\*--------------------*/

	UPROPERTY()
	bool bIsBeingDestroyed;

private:

	//Used to check if the voidsong is off cooldown.
	UPROPERTY()
	bool CanActivateAgain = true;

	/*Set Defaults*\
	\*------------*/

	//The length of Voidsongs. 
	//static const int VoidsongLength = 3;

public:

	//The activation key combo of the voidsong.
	UPROPERTY(EditDefaultsOnly)
	FVoidsongInputs ActivationCombo;
};
