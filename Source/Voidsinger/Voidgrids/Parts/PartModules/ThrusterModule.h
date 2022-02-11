// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActivatablePartModule.h"
#include "ThrusterModule.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UThrusterModule : public UActivatablePartModule
{
	GENERATED_BODY()

public:

	/**
	 * Initializes the part module's protected and private variables
	 *
	 * @param OwningPart - The part that this module is being initialized from
	 */
	virtual void InitializeVariables(UPart* OwningPart) override;

	/* ------------------------ *\
	\* \/ Thrust Predictions \/ */

	/**
	 * Predicts the time it will take to reach a certain linear velocity given the Voidgrid's thrusters.
	 * 
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target velocity to predict the time to reach.
	 * @return The time it will take to reach a certain linear velocity. Returns -1 if it is impossible to reach the target velocity.
	 */
	UFUNCTION(BlueprintPure)
	static float TimeToLinearVelocity(const AVoidgrid* Target, const FVector2D Velocity);

	/**
	 * Predicts the time it will take to reach a certain location given the Voidgrid's thrusters.
	 *
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target location to predict the time to reach.
	 * @return The time it will take to reach a certain location. Returns -1 if it is impossible to reach the target location.
	 */
	UFUNCTION(BlueprintPure)
	static float TimeToLocation(const AVoidgrid* Target, const FVector2D Location);

	/**
	 * Predicts the time it will take to reach a certain angular velocity given the Voidgrid's thrusters.
	 *
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target velocity to predict the time to reach.
	 * @return The time it will take to reach a certain angular velocity. Returns -1 if it is impossible to reach the target velocity.
	 */
	UFUNCTION(BlueprintPure)
	static float TimeToAngularVelocity(const AVoidgrid* Target, const float Velocity);

	/**
	 * Predicts the time it will take to reach a certain orientation given the Voidgrid's thrusters.
	 *
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target orientation to predict the time to reach.
	 * @return The time it will take to reach a certain orientation. Returns -1 if it is impossible to reach the target orientation.
	 */
	UFUNCTION(BlueprintPure)
	static float TimeToOrientation(const AVoidgrid* Target, const float Orientation);

private:
	/*UFUNCTION()
	static FVector2D GetMaximumAccelerationInDirection(const AVoidgrid* Target, const FVector2D Direction);
	static FVector2D GetMaximumAccelerationInDirection(const AVoidgrid* Target, const float DirectionAngle);

	UFUNCTION()
	static FVector2D GetMaximumAccelerationInRotation(const AVoidgrid* Target, const bool bClockwise);
	*/
	UFUNCTION()
	void UpdateThrustPredictions(float Mass, FVector2D CenterOfMass, float MomentOfInertia);

	/* /\ Thrust Predictions /\ *\
	\* ------------------------ */
protected:
	//The force of the impulse applied by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0"))
	float ThrustForce{ 100 };

	//The direction of the impulse applyed by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPreserveRatio))
	FVector2D ThrustDirection{ FVector2D(1,0) };

	//The location of the impulse applyed by this.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint ThrustLocation{ FIntPoint::ZeroValue };

	//Whether or not to apply the voidsong effect to the thrust location while active.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bApplyVoidsongEffect{ true };


protected:
	/**
	 * Called when the owning part is activated and calls the functionality of this part.
	 * Applies an impulse to the voidgrid in an atempt to get it to move in teh vector dirction and face in the roatation direction.
	 */
	virtual void OnActivate_Implementation(const FPartActivationData Data) override;

	/* ------------------- *\
	\* \/ Functionality \/ */

protected:
	/**
	 * Updates voidgrid thrust capabilities.
	 */
	virtual void FunctionalityRestored() override;

	/**
	 * Updates voidgrid thrust capabilities.
	 */
	virtual void FunctionalityLost() override;

	/* /\ Functionality /\ *\
	\* ------------------- */
};
