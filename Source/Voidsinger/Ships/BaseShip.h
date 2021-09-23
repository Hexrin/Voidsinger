// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "ShipPhysicsComponent.h"
#include "ShipMovementComponent.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Components/SceneComponent.h"
#include "BaseShip.generated.h"

class UBaseResourceSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLaserDelegate, float, Damage, float, Duration);

UCLASS()
class VOIDSINGER_API ABaseShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	float const GetFluidCapacity(TEnumAsByte<EResourceType> Fluid);

	UFUNCTION(BlueprintPure)
	float const GetFluidAmount(TEnumAsByte<EResourceType> Fluid);

	UFUNCTION()
	void AddResourceSystem(UBaseResourceSystem* System);

	UFUNCTION()
	void RemoveResourceSystem(UBaseResourceSystem* System);

	//Adds a new Voidsong to the AvailableVoidsongs. Will be useful for gaining a new voidsong.
	UFUNCTION(BlueprintCallable)
	void AddNewVoidsong(TSubclassOf<UBaseVoidsong> Voidsong);

	//Plays a voidsong with a given activation sequence
	UFUNCTION()
	void PlayVoidsong(TArray<int> Sequence);

	//Creates Voidsong objects with the given classes of Voidsongs and adds them to the AvaialableVoidsongs. Will be useful for loading from a save game.
	UFUNCTION()
	void LoadVoidsongs(TArray<TSubclassOf<UBaseVoidsong>> Voidsongs);

	//Event dispatcher for laser.
	UFUNCTION(BlueprintCallable)
	void CallLaser(float Damage, float Duration);

	UPROPERTY()
	TArray<UBaseResourceSystem*> ResourceSystems;

//protected:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPartGridComponent* PartGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShipPhysicsComponent* PhysicsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UShipMovementComponent* MovementComponent;

	//Array of the voidsongs that are available to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBaseVoidsong*> AvailableVoidsongs;

	//For the Event Dispatcher
	UPROPERTY(BlueprintAssignable)
	FLaserDelegate OnLaserDelegate;

};
