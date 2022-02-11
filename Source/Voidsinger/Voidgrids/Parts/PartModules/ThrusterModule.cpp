// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrusterModule.h"

#include "Voidsinger/Voidgrids/Voidgrid.h"

/**
 * Initializes the part module's protected and private variables
 *
 * @param OwningPart - The part that this module is being initialized from
 */
void UThrusterModule::InitializeVariables(UPart* OwningPart)
{
	Super::InitializeVariables(OwningPart);
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
float UThrusterModule::TimeToLinearVelocity(const AVoidgrid* Target, const FVector2D Velocity)
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
float UThrusterModule::TimeToLocation(const AVoidgrid* Target, const FVector2D Location)
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
float UThrusterModule::TimeToAngularVelocity(const AVoidgrid* Target, const float Velocity)
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
float UThrusterModule::TimeToOrientation(const AVoidgrid* Target, const float Orientation)
{
	return 0;
}

//FVector2D UThrusterModule::GetMaximumAccelerationInDirection(const AVoidgrid* Target, const FVector2D Direction)
//{
//	return FVector2D::ZeroVector;
//}

void UThrusterModule::UpdateThrustPredictions(float Mass, FVector2D CenterOfMass, float MomentOfInertia)
{
	
}

/* /\ Thrust Predictions /\ *\
\* ------------------------ */

/**
 * Called when the owning part is activated and calls the functionality of this part.
 * Applies an impulse to the voidgrid in an atempt to get it to move in teh vector dirction and face in the roatation direction.
 */
void UThrusterModule::OnActivate_Implementation(const FPartActivationData Data)
{
	FVector2D RotatedThrustDirection = UPartRotationFunctions::RotateLocation(ThrustDirection, Part->GetTransform().Rotation);

	//if (ThrusterModules.IsEmpty())
	//{
	//}
}
