// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ThrustManager.generated.h"

class AVoidgrid;

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UThrustManager : public UObject
{
	GENERATED_BODY()

	UThrustManager();

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
	float TimeToLinearVelocity(const FVector2D Velocity) const;

	/**
	 * Predicts the time it will take to reach a certain location given the Voidgrid's thrusters.
	 *
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target location to predict the time to reach.
	 * @return The time it will take to reach a certain location. Returns -1 if it is impossible to reach the target location.
	 */
	UFUNCTION(BlueprintPure)
	float TimeToLocation(const FVector2D Location) const;

	/**
	 * Predicts the time it will take to reach a certain angular velocity given the Voidgrid's thrusters.
	 *
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target velocity to predict the time to reach.
	 * @return The time it will take to reach a certain angular velocity. Returns -1 if it is impossible to reach the target velocity.
	 */
	UFUNCTION(BlueprintPure)
	float TimeToAngularVelocity(const float Velocity) const;

	/**
	 * Predicts the time it will take to reach a certain orientation given the Voidgrid's thrusters.
	 *
	 * @param Target - The voidgrid to predict the motion of.
	 * @param Velocity - The target orientation to predict the time to reach.
	 * @return The time it will take to reach a certain orientation. Returns -1 if it is impossible to reach the target orientation.
	 */
	UFUNCTION(BlueprintPure)
	float TimeToOrientation(const float Orientation) const;

private:
	/*UFUNCTION()
	static FVector2D GetMaximumAccelerationInDirection(const AVoidgrid* Target, const FVector2D Direction);
	static FVector2D GetMaximumAccelerationInDirection(const AVoidgrid* Target, const float DirectionAngle);

	UFUNCTION()
	static FVector2D GetMaximumAccelerationInRotation(const AVoidgrid* Target, const bool bClockwise);
	*/
	UFUNCTION()
	void UpdateThrustPredictions(float Mass, FVector2D CenterOfMass, float MomentOfInertia);

	UPROPERTY()
	AVoidgrid* Voidgrid = nullptr;

	/* /\ Thrust Predictions /\ *\
	\* ------------------------ */

};
