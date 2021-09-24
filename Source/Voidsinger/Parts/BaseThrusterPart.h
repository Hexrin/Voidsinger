// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.h"
#include "DrawDebugHelpers.h" //remove this once debuging is compleate
#include "Voidsinger/Ships/ShipPhysicsComponent.h"
#include "BaseThrusterPart.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseThrusterPart : public UBasePart
{
	GENERATED_BODY()
	

public:
	UBaseThrusterPart();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	const float GetThrustForce();

	UFUNCTION(BlueprintPure)
	const float GetThrustRotation();

	UFUNCTION(BlueprintPure)
	const FVector2D GetThrustLocation();

	//Apply force in the direction of the thruster 
	UFUNCTION(BlueprintCallable)
	void Thrust();

	UFUNCTION(BlueprintPure)
	const bool IsThrusting();

protected:
	UPROPERTY(EditDefaultsOnly)
	float ThrustForce;

	UPROPERTY(EditDefaultsOnly)
	FVector2D ThrustLocation;

	UPROPERTY(EditDefaultsOnly)
	float ThrustRotation;

private:
	UPROPERTY()
	bool bIsThrusting;
};