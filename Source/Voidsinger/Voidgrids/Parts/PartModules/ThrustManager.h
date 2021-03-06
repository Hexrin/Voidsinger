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

	//The maximum impulse force that can be applied by a thrust source.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0"))
	float Force{ 0 };

	//The direction of the impulse applied by a thrust source.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPreserveRatio))
	FVector2D Direction{ FVector2D(1,0) };

	//The location of the impulse applied by a thrust source.
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

	UFUNCTION()
	void AddManagedThrustSource(FThrustSource ThrustSource);
	UFUNCTION()
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
	FVector2D GetMaximumAccelerationInDirection(const FVector2D Direction);
	FVector2D GetMaximumAccelerationInDirection(const float DirectionAngle);

	UFUNCTION()
	FVector2D GetMaximumAccelerationInRotation(const bool bClockwise);

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
