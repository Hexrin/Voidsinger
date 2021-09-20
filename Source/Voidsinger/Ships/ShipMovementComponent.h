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
	void RotateShip(bool Clockwise);

	UFUNCTION(BlueprintCallable)
	void Move(FVector2D Direction);

private:
	UPROPERTY()
	TSet<UBaseThrusterPart*> Thrusters;
};
