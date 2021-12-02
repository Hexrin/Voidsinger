// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.h"
#include "DrawDebugHelpers.h" //remove this once debugging is complete
#include "Voidsinger/Ships/ShipPhysicsComponent.h"
#include "BaseThrusterPart.generated.h"

/**
 * Comment the class -Mabel
 */
UCLASS()
class VOIDSINGER_API UBaseThrusterPart : public UBasePart
{
	GENERATED_BODY()
	

public:

	//Comment -Mabel
	UBaseThrusterPart();

	//Comment -Mabel
	virtual void Tick(float DeltaTime) override;

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const float GetThrustForce();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const float GetThrustRotation();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const FVector2D GetThrustRelativeLocation();

	//Apply force in the direction of the thruster 
	UFUNCTION(BlueprintCallable)
	void Thrust(float NewThrottle);

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const bool IsThrusting();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const float GetThrottle();

protected:

	//Comment -Mabel
	UPROPERTY(EditDefaultsOnly)
	float ThrustForce;

	//Comment -Mabel
	UPROPERTY(EditDefaultsOnly)
	FVector2D ThrustLocation;

	//Comment -Mabel
	UPROPERTY(EditDefaultsOnly)
	float ThrustRotation;

private:

	//Comment -Mabel
	UPROPERTY()
	float Throttle{0};
};