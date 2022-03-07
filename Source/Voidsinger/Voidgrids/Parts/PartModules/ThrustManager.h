// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/PartActivationData.h"
#include "ThrustManager.generated.h"

class AVoidgrid;

USTRUCT(BlueprintType)
struct VOIDSINGER_API FThrustSource
{
	GENERATED_BODY()

	//The maximum inpulse force that can be applied by a thrust source.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0"))
	float Force{ 0 };

	//The direction of the impulse applyed by a thrust source.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPreserveRatio))
	FVector2D Direction{ FVector2D(1,0) };

	//The location of the impulse applyed by a thrust source.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Location{ FIntPoint::ZeroValue };

	FThrustSource(float ThrustForce = 0, FVector2D ThrustDirection = FVector2D(1, 0), FVector2D ThrustLocation = FIntPoint::ZeroValue)
	{
		Force = FMath::Max(ThrustForce, 0.f);
		Direction = ThrustDirection.GetSafeNormal();
		Location = ThrustLocation;
	}
};

//Delegate that broadcasts a thrust request containing the effectiveness, move direction, and move rotation
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrustRequest, FPartActivationData, Data);

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
public:
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
	 * @param Location - The target location to predict the time to reach.
	 * @return The time it will take to reach a certain location. Returns -1 if it is impossible to reach the target location.
	 */
	UFUNCTION(BlueprintPure)
	float TimeToLocation(const FVector2D Location, const bool bAccelerating) const;

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
	 * @param Orientation - The target orientation to predict the time to reach.
	 * @param bAccelerating - Whether or not to factor in the voidgrids acceleration capabilites into the calculations
	 * @return The time it will take to reach a certain orientation. Returns -1 if it is impossible to reach the target orientation.
	 */
	UFUNCTION(BlueprintPure)
	float TimeToOrientation(const float Orientation, const bool bAccelerating) const;

	/**
	* Finds the Direction of the Thrust based on the Direction and force of the ThrustSource
	* @param ThrustDirection - The Direction of the Thruster 
	* @param ThrustSource - The Thruster the function is using to get the direction of the Thrust
	* @return The Direction of the thrust
	*/
	UFUNCTION(BlueprintPure)
	FVector2D GetThrustDirection(const FVector2D ThrustDirection, FThrustSource ThrustSource) const;

	/**
	* Finds the Rotation of the Thrust Source given whether the Thruster is thrusting in a Clockwise or CounterClockwise direction
	* @param bClockwise - Return true if thrusting in a Clockwise direction, return false if thrusting in a CounterClockwise direction
	* @param ThrustSource - The Thruster that the function is using to get the rotation of the Thrust
	* @return the Rotation of the thrust 
	*/
	UFUNCTION(BlueprintPure)
	float GetThrustRotation(const bool bClockwise, FThrustSource ThrustSource) const;

	UFUNCTION(BlueprintCallable)
	void AddManagedThrustSource(FThrustSource ThrustSource);
	UFUNCTION(BlueprintCallable)
	void RemoveManagedThrustSource(FThrustSource ThrustSource);

private:
	UPROPERTY()
	float ForwardThrust { 0 };

	UPROPERTY()
	float BackwardThrust{ 0 };
	
	UPROPERTY()
	float RightThrust{ 0 };
	
	UPROPERTY()
	float LeftThrust{ 0 };
	
	UPROPERTY()
	float ClockwiseThrust{ 0 };
	
	UPROPERTY()
	float CounterClockwiseThrust{ 0 };
	
	UFUNCTION()
	float GetMaximumAccelerationInDirection(const FVector2D Direction) const;
	float GetMaximumAccelerationInDirection(const float DirectionAngle) const;

	UFUNCTION()
	float GetMaximumAccelerationInRotation(const bool bClockwise) const;

	UPROPERTY()
	const AVoidgrid* Voidgrid;

	/* /\ Thrust Predictions /\ *\
	\* ------------------------ */

	/* --------------- *\
	\* \/ Delegates \/ */

public:

	UPROPERTY()
	FThrustRequest OnThrustRequest;

	/* /\ Delegates /\ *\
	\* --------------- */
};
