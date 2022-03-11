// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/PartActivationData.h"
#include "ThrustManager.generated.h"

class AVoidgrid;

/**
 * Stores information about a thruster or other thrust source.
 */
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

	/**
	 * Creates a thrust source with the given stats.
	 * 
	 * @param ThrustForce - The maximum force this thrust source can apply.
	 * @param ThrustDirection - The direction of the force applied by this thrust source.
	 * @param ThrustLocaiton - The location of the force relative to the center of the object being thrusted upon.
	 */
	FThrustSource(float ThrustForce = 0, FVector2D ThrustDirection = FVector2D(1, 0), FVector2D ThrustLocation = FIntPoint::ZeroValue)
	{
		Force = FMath::Max(ThrustForce, 0.f);
		Direction = ThrustDirection.GetSafeNormal();
		Location = ThrustLocation;
	}

	/**
	 * Converts this thrust source to a string.
	 * 
	 * @return A string containing all the data of this.
	 */
	FString ToString() const
	{
		FString ReturnValue = FString::SanitizeFloat(Force);
		ReturnValue += Direction.ToString() + Location.ToString();
		return ReturnValue;
}

	/**
	 * Test to see if two thrust sources components are all equal.
	 */
	bool operator==(const FThrustSource& Other) const
	{
		return Force == Other.Force && Direction == Other.Direction && Location == Other.Location;
	}
};

//Hash function for FThrustSource
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FThrustSource& Thing);
#else // optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const FThrustSource& Thing)
{
	return FTextLocalizationResource::HashString(Thing.ToString());
}
#endif

//Delegate that broadcasts a thrust request containing the effectiveness, move direction, and move rotation
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrustRequest, FPartActivationData, Data);

/**
 * A class for managing a voidgrid's thrust sources. 
 * Can make predictions about acceleration and make thrust requests.
 */
UCLASS()
class VOIDSINGER_API UThrustManager : public UObject
{
	GENERATED_BODY()

	/**
	 * Initializes the voidgrid reference.
	 */
	UThrustManager();

	/* ------------------------ *\
	\* \/ Thrust Predictions \/ */
public:
	/**
	 * Predicts the direction of thrust and the time it will take to reach a certain linear velocity given the available thrust sources.
	 * 
	 * @param TimeToVelocity -  Will be set to the time to reach the given velocity. Set to -1 if velocity is unachievable.
	 * @param DirectionToVelocity - Set to the direction acceleration is needed in.
	 * @param TargetVelocity - The velocity to predict the thrust needed to achieve.
	 */
	UFUNCTION(BlueprintPure)
	void PredictThrustToLinearVelocity(float& TimeToVelocity, FVector2D& DirectionToVelocity, const FVector2D TargetVelocity) const;

	/**
	 * Predicts the direction of thrust and the time it will take to reach a certain location. Assumes the Voidgrid is not accelerating.
	 *
	 * @param TimeToVelocity -  Will be set to the time to reach the given location. Will be set to < 0 if location is unachievable.
	 * @param DirectionToVelocity - Set to the direction acceleration is needed in.
	 * @param TargetLocation - The location to predict the thrust needed to arrive at.
	 * @param ErrorRadius - The radius around the target location that count as being at .
	 * @param bAccelerating - Whether or not to include the acceleration capabilities of this voidgrid in the predictions.
	 */
	UFUNCTION(BlueprintPure)
	void PredictThrustToLocation(float& TimeToLocation, FVector2D& DirectionToLocation, const FVector2D TargetLocation, const float ErrorRadius = 0.f) const;

	/**
	 * Predicts the direction of thrust and the time it will take to reach a certain angular velocity given the available thrust sources.
	 *
	 * @param TimeToVelocity -  Will be set to the time to reach the given location. Set to -1 if location is unachievable.
	 * @param bClockwiseToVelocity - Whether or not the voidgrid needs to accelerate in the clockwise direction to reach the target velocity.
	 * @param TargetVelocity - The angular velocity in radians per second to predict the thrust needed to arrive at.
	 */
	UFUNCTION(BlueprintPure)
	void PredictThrustToAngularVelocity(float& TimeToVelocity, bool& bClockwiseToVelocity, const float TargetVelocity) const;

	/**
	 * Predicts the direction of thrust and the time it will take to reach a certain orientation given the available thrust sources.
	 *
	 * @param TimeToVelocity -  Will be set to the time to reach the given location. Set to -1 if location is unachievable.
	 * @param bClockwiseToVelocity - Whether or not the voidgrid needs to accelerate in the clockwise direction to reach the target velocity.
	 * @param TargetOrientation - The orientation in radians to predict the thrust needed to arrive at.
	 * @param bAccelerating - Whether or not to include the acceleration capabilities of this voidgrid in the predictions.
	 */
	UFUNCTION(BlueprintPure)
	void PredictThrustToOrientation(float& TimeToOrientation, bool& bClockwiseToOrientation, const float TargetOrientation, const bool bAccelerating = true) const;

	/**
	 * Gets the maximum possible acceleration that a given thrust source can cause in a given direction on this voidgrid.
	 * 
	 * @param Direction - The direction in local space to get the possible acceleration in. 
	 * @param ThrustSource - The source of thrust used in calculations.
	 * @return The magnitude of the acceleration in grid tiles / second^2.
	 */
	UFUNCTION(BlueprintPure)
	float GetThrustSourceAccelerationInDirection(const FVector2D Direction, const FThrustSource ThrustSource) const;

	/**
	 * Gets the maximum possible acceleration that a given thrust source can cause in a given angular direction on this voidgrid.
	 *
	 * @param bClockwise - The angular direction to get the possible acceleration in.
	 * @param ThrustSource - The source of thrust used in calculations.
	 * @return The acceleration in radians / second^2.
	 */
	UFUNCTION(BlueprintPure)
	float GetThrustSourceAccelerationInRotation(const bool bClockwise, const FThrustSource ThrustSource) const;

	/**
	 * Adds a thrust source to this for use in thrust prediction calculations.
	 * 
	 * @param ThrustSource - The thrust source to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddManagedThrustSource(FThrustSource ThrustSource);

	/**
	 * Removes a thrust source to this for use in thrust prediction calculations.
	 *
	 * @param ThrustSource - The thrust source to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemoveManagedThrustSource(FThrustSource ThrustSource);

private:
	//The thrust force that can be applied in the forward direction.
	UPROPERTY()
	float ForwardThrust { 0 };

	//The thrust force that can be applied in the backward direction.
	UPROPERTY()
	float BackwardThrust{ 0 };

	//The thrust force that can be applied in the left direction.
	UPROPERTY()
	float RightThrust{ 0 };

	//The thrust force that can be applied in the right direction.
	UPROPERTY()
	float LeftThrust{ 0 };

	//All of the thrust sources managed by this .
	UPROPERTY()
	TSet<FThrustSource> ManagedThrustSources;
	
	// \/ GetMaximumAccelerationInDirection \/ //

	/**
	 * Gets the maximum acceleration that all managed thrust sources can provide in a given direction.
	 * 
	 * @param Direction - A vector pointing in the target direction.
	 * @return The acceleration in grid tile / second^2.
	 */
	UFUNCTION()
	float GetMaximumAccelerationInDirection(const FVector2D Direction) const;

	/**
	 * Gets the maximum acceleration that all managed thrust sources can provide in a given direction.
	 *
	 * @param DirectionAngle - The rotation of the target direction.
	 * @return The acceleration in grid tile / second^2.
	 */
	float GetMaximumAccelerationInDirection(const float DirectionAngle) const;

	// /\ GetMaximumAccelerationInDirection /\ //

	/**
	 * Gets the maximum acceleration that all managed thrust sources can provide in a given direction.
	 *
	 * @param bClockwise - Whether or not to get the rotation in the clockwise direction.
	 * @return The acceleration in radians / second^2.
	 */
	UFUNCTION()
	float GetMaximumAccelerationInRotation(const bool bClockwise) const;

	/* /\ Thrust Predictions /\ *\
	\* ------------------------ */

	/* -------------------- *\
	\* \/ Thrust Control \/ */

public:

	//Called when this makes a thrust request. Vector will be in the direction of the target move direction, and rotation will be the target orientation.
	UPROPERTY()
	FThrustRequest OnThrustRequest;

	/* /\ Thrust Control /\ *\
	\* -------------------- */

private:
	//A reference to the voidgrid, whose thrust is being manged by this
	UPROPERTY()
	const AVoidgrid* Voidgrid;
};
