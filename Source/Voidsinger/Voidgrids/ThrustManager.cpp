// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrustManager.h"

#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ThrusterModule.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

UThrustManager::UThrustManager()
{
	Voidgrid = Cast<AVoidgrid>(GetOuter());
}

/* ------------------------ *\
\* \/ Thrust Predictions \/ */

/**
 * Predicts the direction of thrust and the time it will take to reach a certain linear velocity given the available thrust sources.
 *
 * @param TimeToVelocity -  Will be set to the time to reach the given velocity. Set to -1 if velocity is unachievable.
 * @param DirectionToVelocity - Set to the direction acceleration is needed in.
 * @param TargetVelocity - The velocity to predict the thrust needed to achieve.
 */
void UThrustManager::PredictThrustToLinearVelocity(float& TimeToVelocity, FVector2D& DirectionToVelocity, const FVector2D TargetVelocity) const
{
	FVector2D DeltaVelocity = (TargetVelocity - (Voidgrid->LinearVelocity));
	TimeToVelocity = (DeltaVelocity).Size() / GetMaximumAccelerationInDirection(DeltaVelocity);
}

/**
 * Predicts the direction of thrust and the time it will take to reach a certain location. Assumes the Voidgrid is not accelerating.
 *
 * @param TimeToVelocity -  Will be set to the time to reach the given location. Will be set to < 0 if location is unachievable.
 * @param DirectionToVelocity - Set to the direction acceleration is needed in.
 * @param TargetLocation - The location to predict the thrust needed to arrive at.
 * @param ErrorRadius - The radius around the target location that count as being at .
 * @param bAccelerating - Whether or not to include the acceleration capabilities of this voidgrid in the predictions.
 */
void UThrustManager::PredictThrustToLocation(float& TimeToLocation, FVector2D& DirectionToLocation, const FVector2D TargetLocation, const float ErrorRadius = 0.f) const
{
	FVector2D DeltaPosition = TargetLocation - FVector2D(Voidgrid->GetActorLocation());

	//The speed at which the voidgrid is moving
	float VoidgridSpeed = Voidgrid->LinearVelocity.Size();

	//If voidgrid will arrive within the error radius of the target location.
	if (abs(DeltaPosition ^ Voidgrid->LinearVelocity) / VoidgridSpeed <= ErrorRadius)
	{
		TimeToLocation = (DeltaPosition | Voidgrid->LinearVelocity) / FMath::Square(VoidgridSpeed);
	}
	else
	{
		TimeToLocation = -1;
	}

	//The direction to the target location.
	FVector2D TargetDirection = DeltaPosition.GetSafeNormal();

	//If not pointing in correct direction return direction that will adjust velocity to be in the correct direction.
	DirectionToLocation = TimeToLocation > 0 ? TargetDirection : (TargetDirection - (Voidgrid->LinearVelocity / VoidgridSpeed)).GetSafeNormal();
}

/**
 * Predicts the direction of thrust and the time it will take to reach a certain angular velocity given the available thrust sources.
 *
 * @param TimeToVelocity -  Will be set to the time to reach the given location. Set to -1 if location is unachievable.
 * @param bClockwiseToVelocity - Whether or not the voidgrid needs to accelerate in the clockwise direction to reach the target velocity.
 * @param TargetVelocity - The angular velocity in radians per second to predict the thrust needed to arrive at.
 */
UFUNCTION(BlueprintPure)
void UThrustManager::PredictThrustToAngularVelocity(float& TimeToVelocity, bool& bClockwiseToVelocity, const float TargetVelocity) const
{ 
	float DeltaVelocity = TargetVelocity - Voidgrid->AngularVelocity;
	bClockwiseToVelocity = TargetVelocity > 0;
	TimeToVelocity = abs(DeltaVelocity) / GetMaximumAccelerationInDirection(bClockwiseToVelocity);
}

/**
 * Predicts the direction of thrust and the time it will take to reach a certain orientation given the available thrust sources.
 *
 * @param TimeToVelocity -  Will be set to the time to reach the given location. Set to -1 if location is unachievable.
 * @param bClockwiseToVelocity - Whether or not the voidgrid needs to accelerate in the clockwise direction to reach the target velocity.
 * @param TargetOrientation - The orientation in radians to predict the thrust needed to arrive at.
 * @param bAccelerating - Whether or not to include the acceleration capabilities of this voidgrid in the predictions.
 */
UFUNCTION(BlueprintPure)
void UThrustManager::PredictThrustToOrientation(float& TimeToOrientation, bool& bClockwiseToOrientation, const float TargetOrientation, const bool bAccelerating = true) const
{
	//The angular distance form current orientation to the target orientation in the counterclockwise direction.
	float CounterClockwiseAngularDistance = FMath::Fmod(TargetOrientation > 0 ? TargetOrientation - Voidgrid->GetActorQuat().GetAngle() : 2 * PI - (TargetOrientation - Voidgrid->GetActorQuat().GetAngle()), 2 * PI);
	//The angular distance form current orientation to the target orientation in the clockwise direction.
	float ClockwiseAngularDistance = PI - CounterClockwiseAngularDistance;

	bClockwiseToOrientation = ClockwiseAngularDistance < CounterClockwiseAngularDistance;
	if (bAccelerating)
	{
		// \/ Uses the quadratic formula to solve for the time to orientation \/ //

		//The acceleration in the direction with the least angular distance.
		float Acceleration = GetMaximumAccelerationInRotation(bClockwiseToOrientation);

		if (Acceleration > 0)
		{
			//The velocity in the same direction as the acceleration.
			float AccellerationRelativeVelocity = bClockwiseToOrientation ? Voidgrid->AngularVelocity : Voidgrid->AngularVelocity * -1;
			//The distance in the same direction as the acceleration
			float AccelerationRelativeDistance = bClockwiseToOrientation ? ClockwiseAngularDistance : CounterClockwiseAngularDistance;

			TimeToOrientation = (-AccellerationRelativeVelocity + FMath::Sqrt(FMath::Square(AccellerationRelativeVelocity) + 2 * Acceleration * AccelerationRelativeDistance)) / Acceleration;
			return;
		}
		// /\ Uses the quadratic formula to solve for the time to orientation /\ //
	}

	//Angular distance / angular velocity
	TimeToOrientation = (Voidgrid->AngularVelocity > 0 ? CounterClockwiseAngularDistance : ClockwiseAngularDistance) / Voidgrid->AngularVelocity;
}

/**
 * Gets the maximum possible acceleration that a given thrust source can cause in a given direction on this voidgrid.
 *
 * @param Direction - The direction in local space to get the possible acceleration in.
 * @param ThrustSource - The source of thrust used in calculations.
 * @return The magnitude of the acceleration.
 */
float UThrustManager::GetThrustSourceAccelerationInDirection(const FVector2D Direction, const FThrustSource ThrustSource) const
{
	if (Voidgrid->Mass > 0)
	{
		return FMath::Max(ThrustSource.Direction | Direction * ThrustSource.Force, 0.f) / Voidgrid->Mass;
	}
	return 0;
}

/**
 * Gets the maximum possible acceleration that a given thrust source can cause in a given angular direction on this voidgrid.
 *
 * @param bClockwise - The angular direction to get the possible acceleration in.
 * @param ThrustSource - The source of thrust used in calculations.
 * @return The magnitude of the acceleration.
 */
float UThrustManager::GetThrustSourceAccelerationInRotation(const bool bClockwise, const FThrustSource ThrustSource) const
{
	if (Voidgrid->MomentOfInertia > 0)
	{
		float RotationalEffectivness = ((ThrustSource.Location - Voidgrid->CenterOfMass) ^ ThrustSource.Direction) * ThrustSource.Force / Voidgrid->MomentOfInertia;
		if (bClockwise)
		{
			return FMath::Max(RotationalEffectivness, 0);
		}
		else
		{
			return FMath::Max(RotationalEffectivness * -1.f, 0);
		}
	}
	return 0;
}

/**
 * Adds a thrust source to this for use in thrust prediction calculations.
 *
 * @param ThrustSource - The thrust source to add.
 */
void UThrustManager::AddManagedThrustSource(FThrustSource ThrustSource)
{
	//Get linear thrust vectors
	(ThrustSource.Direction.X > 0 ? ForwardThrust : BackwardThrust) += abs(ThrustSource.Direction.X) * ThrustSource.Force;
	(ThrustSource.Direction.Y > 0 ? RightThrust : LeftThrust) += abs(ThrustSource.Direction.Y) * ThrustSource.Force;

	//Get rotational thrust vectors
	float RotationalEffectivness = (ThrustSource.Location - Voidgrid->CenterOfMass) ^ ThrustSource.Direction;
	if (FMath::IsNearlyZero(RotationalEffectivness, .01f))
	{
		(RotationalEffectivness > 0 ? ClockwiseThrust : CounterClockwiseThrust) += abs(RotationalEffectivness) * ThrustSource.Force;
	}
}

/**
 * Removes a thrust source to this for use in thrust prediction calculations.
 *
 * @param ThrustSource - The thrust source to remove.
 */
void UThrustManager::RemoveManagedThrustSource(FThrustSource ThrustSource)
{
	//Get linear thrust vectors
	(ThrustSource.Direction.X > 0 ? ForwardThrust : BackwardThrust) -= abs(ThrustSource.Direction.X) * ThrustSource.Force;
	(ThrustSource.Direction.Y > 0 ? RightThrust : LeftThrust) -= abs(ThrustSource.Direction.Y) * ThrustSource.Force;

	//Get rotational thrust vectors
	float RotationalEffectivness = (ThrustSource.Location - Voidgrid->CenterOfMass) ^ ThrustSource.Direction;
	if (FMath::IsNearlyZero(RotationalEffectivness, .01f))
	{
		(RotationalEffectivness < 0 ? ClockwiseThrust : CounterClockwiseThrust) -= abs(RotationalEffectivness) * ThrustSource.Force;
	}
}

// \/ GetMaximumAccelerationInDirection \/ //

/**
 * Gets the maximum acceleration that all managed thrust sources can provide in a given direction.
 *
 * @param Direction - A vector pointing in the target direction.
 * @return The acceleration in grid tile / second^2.
 */
float UThrustManager::GetMaximumAccelerationInDirection(const FVector2D Direction) const
{
	if (Voidgrid->Mass > 0)
	{
		const FVector2D NormailizedDirection = Direction.GetSafeNormal();
		return ((NormailizedDirection.X > 0 ? ForwardThrust : BackwardThrust) * NormailizedDirection.X + (NormailizedDirection.Y > 0 ? RightThrust : LeftThrust) * NormailizedDirection.Y) / Voidgrid->Mass;
	}
	return 0;
}

/**
 * Gets the maximum acceleration that all managed thrust sources can provide in a given direction.
 *
 * @param DirectionAngle - The rotation of the target direction.
 * @return The acceleration in grid tile / second^2.
 */
float UThrustManager::GetMaximumAccelerationInDirection(const float DirectionAngle) const
{
	return GetMaximumAccelerationInDirection(FVector2D(FMath::Cos(DirectionAngle), FMath::Sin(DirectionAngle)));
}

// /\ GetMaximumAccelerationInDirection /\ //

float UThrustManager::GetMaximumAccelerationInRotation(const bool bClockwise) const
{
	return bClockwise ? ClockwiseThrust : CounterClockwiseThrust;
}


/* /\ Thrust Predictions /\ *\
\* ------------------------ */