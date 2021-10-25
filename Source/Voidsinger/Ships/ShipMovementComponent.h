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
	const float GetLookDirectionErrorTollerance();

	UFUNCTION(BlueprintPure)
	const float GetMoveSpeedErrorTollerance();

	UFUNCTION(BlueprintPure)
	const float GetRotationDirectionUpdateInterval();

	UFUNCTION(BlueprintPure)
	TSet<UBaseThrusterPart*> GetThrustersForDirection(FVector2D Direction);

	UFUNCTION(BlueprintPure)
	TSet<UBaseThrusterPart*> GetThrustersForRotation(bool Clockwise);

	UFUNCTION(BlueprintPure)
	const float GetMaximumAccelerationInDirection(FVector2D Direction, float AtThrottle = 1);

	UFUNCTION(BlueprintPure)
	const float GetMaximumAccelerationInRotation(bool Clockwise, float AtThrottle = 1);

protected:
	//In radians
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float LookDirectionErrorTollerance{ 0.075 };

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float MoveSpeedErrorTollerance{ 0.5 };

	//The frequency at which ships alternate between accelerating and decelerating what trying to look in a direction
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	float RotationDirectionUpdateInterval{ 0.1 };

private:
	UPROPERTY()
	TSet<UBaseThrusterPart*> Thrusters;

	TMap<bool, TSet<UBaseThrusterPart*>> ThrustersForRotation;
	TMap<FVector2D, TSet<UBaseThrusterPart*>> ThrustersForDirection;
};
