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

UCLASS(Abstract)
class VOIDSINGER_API UBaseVoidsong : public UObject
{
	GENERATED_BODY()

public:

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*-Defaults-*\
	\*----------*/

	//The name of the Voidword that the player will see.
	static const FText VoidsongDisplayText;

	/*
	* UE doesnt like int, use int32 or int64 instead.
	* - Liam Suggestion
	*/
	//The sequence of numbers that when played will activate the Voidword.
	static const TArray<int32> ActivationCombo;

	//Copy comment form GetDuration() - Liam Suggestion
	//The amount of time that this Voidword takes to play
	static const float Duration;

};
