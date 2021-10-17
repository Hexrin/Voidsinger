// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShipMovementComponent.generated.h"

class UBaseThrusterPart;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIDSINGER_API UShipMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
	void UpdateThrusters();

	UFUNCTION(BlueprintCallable)
	void RotateShip(bool Clockwise, float Throttle);

	UFUNCTION(BlueprintCallable)
	void Move(FVector2D Direction, float Throttle);
	
	UFUNCTION(BlueprintPure)
	float GetLookDirectionTollerance();

	UFUNCTION(BlueprintPure)
	float GetDecelerationPredictionTime();

	UFUNCTION(BlueprintPure)
	TSet<UBaseThrusterPart*> GetThrustersForDirection(FVector2D Direction);

	UFUNCTION(BlueprintPure)
	TSet<UBaseThrusterPart*> GetThrustersForRotation(bool Clockwise);

	UFUNCTION(BlueprintPure)
	float GetMaximumAccelerationInDirection(FVector2D Direction);

	UFUNCTION(BlueprintPure)
	float GetMaximumAccelerationInRotation(bool Clockwise);

protected:
	UPROPERTY(EditAnywhere)
	float LookDirectionTollerance{ 0.05 };

	//The time into the future used to predict when to stop roatating 
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float DecelerationPredictionTime{ 0.35 };



private:
	UPROPERTY()
	TSet<UBaseThrusterPart*> Thrusters;

	TMap<bool, TSet<UBaseThrusterPart*>> ThrustersForRotation;
	TMap<FVector2D, TSet<UBaseThrusterPart*>> ThrustersForDirection;
};
