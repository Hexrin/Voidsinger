// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseShip.h"
#include "ShipMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOIDSINGER_API UShipMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShipMovementComponent();

protected:

	virtual void BeginPlay() override;

public:	

	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//declare functions
	UFUNCTION(BlueprintCallable)
	void AddForce(FVector2D ForceLocation, FVector2D Force);
	
private:

	//declare private variables
	class ABaseShip* Ship;
	FVector2D Velocity;
	float AngularVelocity;

};
