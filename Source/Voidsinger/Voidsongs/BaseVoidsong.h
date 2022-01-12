// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BaseVoidsong.generated.h"

/*
* This class should not be able to be inscaniated as it has no values that change during runtime
* Aka it should be a static class.
* The only way to create a static class that can be overriden in BP is a blueprint function library, so
* this class should derive from UBlueprintFunctionLibrary and all functions should be made static.
* All variables should be converted to non-UPROPERTY() public static const varaibles.
* Sadly this means that they will not be able to be set in blueprints but we should be using config files anyways.
* 
* If this is done the class should no longer derive from FTickableGameObject.
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

	/*
	* Comment not helpful. What does the constuctor do? 
	* 
	* If suggestion from lines 11-18 are followed this function cannot exist 
	* - Liam Suggestion
	*/ 
	//Constructor
	UBaseVoidsong();

	//Comment not helpful. What does the GetWorld() functionality mean? - Liam Suggestion
	//GetWorld() functionality
	virtual class UWorld* GetWorld() const override;

	/*--------Tick--------*\
	\*--------------------*/

	/*
	* If suggestion from lines 11 - 18 are followed this function cannot exist.
	* Otherwise this function should still be deleted because it is and should never be used.
	* - Liam Suggestion
	*/	
	//Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* If suggestion from lines 11 - 18 are followed then this function cannot exist.
	* Otherwise this function should still be deleted because it is and should never be used.
	* - Liam Suggestion
	*/
	//Determines if this part will tick
	virtual bool IsTickable() const override;

	/*
	* If suggestion from lines 11 - 18 are followed then this function cannot exist.
	* Otherwise this function should still be deleted because it is and should never be used.
	* - Liam Suggestion
	*/
	//Is a nesseary part of tick component
	virtual TStatId GetStatId() const override;

public:
	/*
	* Bad Name.
	* Sould not be called event tick becaue in blueprint it will be called Event Event Tick. 
	* Use OnTick to be consitant with my other classes.
	* 
	* If suggestion from lines 11 - 18 are followed then this function cannot exist.
	* Otherwise this function should still be deleted because it is and should never be used.
	* - Liam Suggestion
	*/
	//Event Tick for use in blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void EventTick(float DeltaTime);

	/*Getters*\
	\*-------*/

	/*
	* UE doesnt like int, use int32 or int64 instead 
	* 
	* If suggestion from lines 11 - 18 are followed then this function is pointless.
	* - Liam Suggestion
	*/
	//Gets the activation combo of this voidsong
	UFUNCTION(BlueprintPure)
	TArray<int> GetActivationCombo();

	//If suggestion from lines 11 - 18 are followed then this function is pointless. - Liam Suggestion
	//Gets the display text of the Voidsong
	UFUNCTION(BlueprintPure)
	FText GetVoidsongDisplayText();

	//If suggestion from lines 11 - 18 are followed then this function is pointless. - Liam Suggestion
	//Should have a short comment explaining what the duration acutaly means or how it is used - Liam Suggestion
	UFUNCTION(BlueprintPure)
	float GetDuration();

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Blueprint Defaults*\
	\*------------------*/

	//The name of the Voidsong that the player will see.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText VoidsongDisplayText;

	/*
	* UE doesnt like int, use int32 or int64 instead.
	* Explain what the activation combo is and what the numbers correspond to.
	* - Liam Suggestion
	*/
	//The activation key combo of the voidsong.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<int> ActivationCombo;

	//Copy comment form GetDuration() - Liam Suggestion
	UPROPERTY(EditDefaultsOnly)
	float Duration;

	/*Instanced  Variables*\
	\*--------------------*/

	//How is this used? Needs comment. - Liam Suggstion
	UPROPERTY()
	bool bIsBeingDestroyed;
};
