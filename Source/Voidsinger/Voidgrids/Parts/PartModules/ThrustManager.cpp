// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrustManager.h"

#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ThrusterModule.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

UThrustManager::UThrustManager()
{
	Voidgrid = Cast<AVoidgrid>(GetOuter());
}

/* ------------------------ *\
\* \/ Thrust Predictions \/ */

/**
 * Predicts the time it will take to reach a certain linear velocity given the Voidgrid's thrusters.
 *
 * @param Target - The voidgrid to predict the motion of.
 * @param Velocity - The target velocity to predict the time to reach.
 * @return The time it will take to reach a certain linear velocity. Returns -1 if it is impossible to reach the target velocity.
 */
float UThrustManager::TimeToLinearVelocity(const FVector2D Velocity) const
{
	return 0;
}

/**
 * Predicts the time it will take to reach a certain location given the Voidgrid's thrusters.
 *
 * @param Target - The voidgrid to predict the motion of.
 * @param Velocity - The target location to predict the time to reach.
 * @return The time it will take to reach a certain location. Returns -1 if it is impossible to reach the target location.
 */
float UThrustManager::TimeToLocation(const FVector2D Location) const
{
	return 0;
}

/**
 * Predicts the time it will take to reach a certain angular velocity given the Voidgrid's thrusters.
 *
 * @param Target - The voidgrid to predict the motion of.
 * @param Velocity - The target velocity to predict the time to reach.
 * @return The time it will take to reach a certain angular velocity. Returns -1 if it is impossible to reach the target velocity.
 */
float UThrustManager::TimeToAngularVelocity(const float Velocity) const
{
	return 0;
}

/**
 * Predicts the time it will take to reach a certain orientation given the Voidgrid's thrusters.
 *
 * @param Target - The voidgrid to predict the motion of.
 * @param Velocity - The target orientation to predict the time to reach.
 * @return The time it will take to reach a certain orientation. Returns -1 if it is impossible to reach the target orientation.
 */
float UThrustManager::TimeToOrientation(const float Orientation) const
{
	return 0;
}

void UThrustManager::AddManagedThrustSource(FThrustSource ThrustSource)
{
	(ThrustSource.Direction.X > 0 ? ForwardThrust : BackwardThrust) = abs(ThrustSource.Direction.X) * ThrustSource.Force;
	(ThrustSource.Direction.Y > 0 ? RightThrust : LeftThrust) = abs(ThrustSource.Direction.Y) * ThrustSource.Force;
	(ThrustSource.Location ^ Voidgrid->CenterOfMass);
}

void UThrustManager::RemoveManagedThrustSource(FThrustSource ThrustSource)
{

}

FVector2D UThrustManager::GetMaximumAccelerationInDirection(const FVector2D Direction)
{
	return 0;
}

FVector2D UThrustManager::GetMaximumAccelerationInDirection(const float DirectionAngle)
{
	return GetMaximumAccelerationInDirection(FVector2D(FMath::Cos(DirectionAngle), FMath::Sin(DirectionAngle)));
}

FVector2D UThrustManager::GetMaximumAccelerationInRotation(const bool bClockwise)
{
	return 0;
}


/* /\ Thrust Predictions /\ *\
\* ------------------------ */