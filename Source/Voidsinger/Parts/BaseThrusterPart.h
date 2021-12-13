// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.h"
#include "DrawDebugHelpers.h" //remove this once debugging is complete
#include "Voidsinger/Ships/ShipPhysicsComponent.h"
#include "BaseThrusterPart.generated.h"

/**
 * Comment the class -Mabel Suggestion
 */
UCLASS()
class VOIDSINGER_API UBaseThrusterPart : public UBasePart
{
	GENERATED_BODY()
	

public:

	//Comment -Mabel Suggestion
	UBaseThrusterPart();

	//Comment -Mabel Suggestion
	virtual void Tick(float DeltaTime) override;

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const float GetThrustForce();

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const float GetThrustRotation();

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const FVector2D GetThrustRelativeLocation();

	//Apply force in the direction of the thruster 
	UFUNCTION(BlueprintCallable)
	void Thrust(float NewThrottle);

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const bool IsThrusting();

	//Comment -Mabel Suggestion
	UFUNCTION(BlueprintPure)
	const float GetThrottle();

protected:

	//Comment -Mabel Suggestion
	UPROPERTY(EditDefaultsOnly)
	float ThrustForce;

	//Comment -Mabel Suggestion
	UPROPERTY(EditDefaultsOnly)
	FVector2D ThrustLocation;

	//Comment -Mabel Suggestion
	UPROPERTY(EditDefaultsOnly)
	float ThrustRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FuelConsumptionAmount{.1};

private:

	//Comment -Mabel Suggestion
	UPROPERTY()
	float Throttle{0};
};