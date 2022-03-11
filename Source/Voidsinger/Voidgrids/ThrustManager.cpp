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
 * Predicts the time it will take to reach a certain orientation given the Voidgrid's thrusters.
 *
 * @param Orientation - The target orientation to predict the time to reach.
 * @param bAccelerating - Whether or not to factor in the voidgrids acceleration capabilities into the calculations
 * @return The time it will take to reach a certain orientation. Returns -1 if it is impossible to reach the target orientation.
 */
float UThrustManager::TimeToOrientation(const float Orientation, const bool bAccelerating) const
{
	float CounterClockwiseAngularDistance = FMath::Fmod(Orientation > 0 ? Orientation - Voidgrid->GetActorQuat().GetAngle() : 2 * PI - (Orientation - Voidgrid->GetActorQuat().GetAngle()), 2 * PI);
	float ClockwiseAngularDistance = PI - CounterClockwiseAngularDistance;

	if (bAccelerating)
	{
		bool bAccelerateClockwise = ClockwiseAngularDistance < CounterClockwiseAngularDistance;
		//Uses the quadratic formula to caluclate the time to orentation if constanly acelerating
		float HalfAccelertion = GetMaximumAccelerationInRotation(bAccelerateClockwise) / 2;
		float AccelerationRelativeVelocity = bAccelerateClockwise ? Voidgrid->AngularVelocity * -1 : Voidgrid->AngularVelocity;
		return (-1 * AccelerationRelativeVelocity + AccelerationRelativeVelocity) / (2 * HalfAccelertion);
	}
	
	return (Voidgrid->AngularVelocity > 0 ? CounterClockwiseAngularDistance : ClockwiseAngularDistance) / Voidgrid->AngularVelocity;
}

FVector2D UThrustManager::GetThrustDirection(const FVector2D ThrustDirection, FThrustSource ThrustSource) const
{
	return FMath::Max((ThrustSource.Direction | ThrustDirection * ThrustSource.Force),0.f);
}

float UThrustManager::GetThrustRotation(const bool bClockwise, FThrustSource ThrustSource) const
{
	float RotationalEffectivness = ((ThrustSource.Location - Voidgrid->CenterOfMass) ^ ThrustSource.Direction)*ThrustSource.Force;
	if (bClockwise) 
	{
		return RotationalEffectivness;
	}
	else
	{
		return RotationalEffectivness* -1.f;
	}
}

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

float UThrustManager::GetMaximumAccelerationInDirection(const FVector2D Direction) const
{
	const FVector2D NormailizedDirection = Direction.GetSafeNormal();
	return (NormailizedDirection.X > 0 ? ForwardThrust : BackwardThrust) * NormailizedDirection.X + (NormailizedDirection.Y > 0 ? RightThrust : LeftThrust) * NormailizedDirection.Y;
}

float UThrustManager::GetMaximumAccelerationInDirection(const float DirectionAngle) const
{
	return GetMaximumAccelerationInDirection(FVector2D(FMath::Cos(DirectionAngle), FMath::Sin(DirectionAngle)));
}

float UThrustManager::GetMaximumAccelerationInRotation(const bool bClockwise) const
{
	return bClockwise ? ClockwiseThrust : CounterClockwiseThrust;
}


/* /\ Thrust Predictions /\ *\
\* ------------------------ */