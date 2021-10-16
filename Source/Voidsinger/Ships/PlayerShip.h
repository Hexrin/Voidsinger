// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/StarSystemData.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "PlayerShip.generated.h"

/**
 * 
 */

UCLASS()
class VOIDSINGER_API APlayerShip : public ABaseShip
{
	GENERATED_BODY()
	


public:

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             FUNCTIONS             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

	/*Initializer Functions*\
	\*---------------------*/

	//Constructor
	APlayerShip();
	
	//Tick
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Begin play
	virtual void BeginPlay() override;

	/*Getter Functions*\
	\*----------------*/

	//Gets the travel cost of going to the given start system
	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, float> GetTravelCost(class UStarSystemData* Target);

	//Gets the start system the player is currently in
	UFUNCTION(BlueprintPure)
	class UStarSystemData* GetCurrentStarSystem();

	/*Traveling Functions*\
	\*-------------------*/

	//Travels to the start system given
	UFUNCTION(BlueprintCallable)
	bool TravelToStarSystem(class UStarSystemData* Target);

	/*Voidsong input actions*\
	\*----------------------*/

	//Input actions
	UFUNCTION()
	void Voidsong1Call();

	UFUNCTION()
	void Voidsong2Call();

	UFUNCTION()
	void Voidsong3Call();

	UFUNCTION()
	void Voidsong4Call();

	UFUNCTION()
	void Voidsong5Call();


	/*Voidsong Management*\
	\*-------------------*/

	//Records the incoming voidsong input
	UFUNCTION()
	void AddVoidsongInput(int Input);

	//Activates the voidsong
	void ActivateVoidsong();

	//Clears the sequence of inputs that the player has made
	UFUNCTION(BlueprintCallable)
	void ResetVoidsong();


	/*-Movement Bindings-*\
	\*-------------------*/

	UFUNCTION()
	void MoveForwardPressedCall();

	UFUNCTION()
	void MoveBackwardPressedCall();

	UFUNCTION()
	void MoveRightPressedCall();

	UFUNCTION()
	void MoveLeftPressedCall();

	UFUNCTION()
	void MoveForwardReleasedCall();

	UFUNCTION()
	void MoveBackwardReleasedCall();

	UFUNCTION()
	void MoveRightReleasedCall();

	UFUNCTION()
	void MoveLeftReleasedCall();


	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//


	/*Voidsong Timers*\
	\*---------------*/

	//The time the player has to wait without playing to reset the Voidsong combo
	UPROPERTY(EditDefaultsOnly)
	float VoidsongResetDelay = 3.0;

	//The timer for counting to VoidsongResetDelay
	UPROPERTY()
	float ResetVoidsongTimer = 0;

	//Boolean for the Voidsong Timer
	UPROPERTY()
	bool ShouldResetVoidsongTimerTick = false;

	/*Voidsong Management*\
	\*-------------------*/

	//Array of the inputs that the player has put in to play voidsongs
	UPROPERTY()
	TArray<int> VoidsongCombo;

	//The spawned metasound to set parameters on
	UPROPERTY(EditAnywhere)
	UAudioComponent* VoidsongInstrument;

	//The voidsong instrument reference. Is set through blueprint.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> VoidsongInstrumentAsset;



	/*------Camera-------*\
	\*-------------------*/
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

protected:
	UPROPERTY(EditAnywhere)
	float CameraHeight;

	/*-----Building------*\
	\*-------------------*/
public:
	UFUNCTION(BlueprintCallable)
	void SetBuildMode(bool NewBuildMode);

	UFUNCTION(BlueprintImplementableEvent)
	void OpenBuildMenu();

	UFUNCTION(BlueprintPure)
	bool IsInBuildMode();

private:
	UPROPERTY()
	bool bBuildMode{ false };



	/*-------Misc.-------*\
	\*-------------------*/

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Pixels;

	UPROPERTY(VisibleAnywhere)
	class UStarSystemData* CurrentStarSystem;
	
};
