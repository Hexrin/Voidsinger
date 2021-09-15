// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/StarSystemData.h"
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
	void AddVoidsongInput(int input);

	//Clears the Voidsong combo so if the player doesn't play for a bit it will just reset
	UFUNCTION()
	void ResetVoidsong();

	//Creates Voidsong objects with the given classes of Voidsongs and adds them to the AvaialableVoidsongs. Will be useful for loading from a save game.
	UFUNCTION()
	void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	//Adds a new Voidsong to the AvailableVoidsongs. Will be useful for gaining a new voidsong.
	UFUNCTION(BlueprintCallable)
	void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\\
	//             VARIABLES             ||
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//


	/*Voidsong Timer*\
	\*--------------*/

	//The time the player has to wait without playing to reset the Voidsong combo
	UPROPERTY(EditDefaultsOnly)
	float VoidsongResetDelay = 3.0;

	//The time for counting to VoidsongResetDelay
	UPROPERTY()
	float ResetVoidsongTimer = 0;

	//Boolean for the Voidsong Timer
	UPROPERTY()
	bool ShouldVoidsongTimerTick = false;

	/*Voidsong Management*\
	\*-------------------*/

	//Array of the inputs that the player has put in to play voidsongs
	UPROPERTY()
	TArray<int> VoidsongCombo;

	//Array of the voidsongs that are available to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBaseVoidsong*> AvailableVoidsongs;

	//idk about everythings after this so liam should commnt these
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Pixels;

	UPROPERTY(EditAnywhere)
	float CameraHeight;

	UPROPERTY(VisibleAnywhere)
	class UStarSystemData* CurrentStarSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;
	
};
