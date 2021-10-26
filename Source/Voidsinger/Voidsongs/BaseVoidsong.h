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

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Blueprint Defaults*\
	\*------------------*/

	//The name of the Voidsong that the player will see.
	UPROPERTY(EditDefaultsOnly)
	FText VoidsongDisplayText;

	/*Instanced  Variables*\
	\*--------------------*/

	UPROPERTY()
	bool bIsBeingDestroyed;

	//The activation key combo of the voidsong.
	UPROPERTY(EditDefaultsOnly)
	TArray<int> ActivationCombo;
};
