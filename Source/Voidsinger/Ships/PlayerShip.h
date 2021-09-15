// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/StarSystemData.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Camera/CameraComponent.h"
#include "PlayerShip.generated.h"

/**
 * 
 */

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidsongDelegate, TArray<int>, ActivationSequence);

UCLASS()
class VOIDSINGER_API APlayerShip : public ABaseShip
{
	GENERATED_BODY()
	

protected:
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Pixels;

	UPROPERTY(EditAnywhere)
	float CameraHeight;

	UPROPERTY(VisibleAnywhere)
	class UStarSystemData* CurrentStarSystem;

public:
	APlayerShip();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, float> GetTravelCost(class UStarSystemData* Target);

	UFUNCTION(BlueprintCallable)
	bool TravelToStarSystem(class UStarSystemData* Target);

	UFUNCTION(BlueprintPure)
	class UStarSystemData* GetCurrentStarSystem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	/*Voidsong activation*\
	\*-------------------*/

	UFUNCTION()
	void AddVoidsongInput(int input);

	UFUNCTION()
	void ResetVoidsong();

	UPROPERTY()
	TArray<int> VoidsongCombo;

	//UPROPERTY()
	//FVoidsongDelegate OnVoidsongDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBaseVoidsong*> AvailableVoidsongs;

	UFUNCTION()
	void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	UFUNCTION(BlueprintCallable)
	void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	UPROPERTY()
	bool ShouldVoidsongTimerTick = false;

	UPROPERTY()
	float VoidsongResetDelay = 3.0;

	UPROPERTY()
	float ResetVoidsongTimer = 0;

};
