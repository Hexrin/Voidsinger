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
 * Predicts the time it will take to reach a certain location given the Voidgrid's thrusters.
 *
 * @param Target - The voidgrid to predict the motion of.
 * @param Location - The target location to predict the time to reach.
 * @return The time it will take to reach a certain location. Returns -1 if it is impossible to reach the target location.
 */
float UThrustManager::TimeToLocation(const FVector2D Location, const bool bAccelerating) const
{
	float c = (Voidgrid->GetActorLocation().Size() - (Location).Size());
	float b = (Voidgrid->LinearVelocity.Size());
	float a = 1;
	float timetoVelocity = 0;

	if (bAccelerating) 
	{
		a = (GetMaximumAccelerationInDirection((Location)-FVector2D(Voidgrid->GetActorLocation())));	
		if ((FVector(Voidgrid->LinearVelocity.GetSafeNormal(), 0) - (UKismetMathLibrary::GetDirectionUnitVector(Voidgrid->GetActorLocation(), FVector(Location, 0)))).Size() != 0)
		{

			if (GetMaximumAccelerationInDirection(FVector2D((FVector(Voidgrid->LinearVelocity, 0)) - (((UKismetMathLibrary::GetDirectionUnitVector(Voidgrid->GetActorLocation(), FVector(Location, 0))))))) != 0)
			{
				return -1;
			}
			else
			{
				timetoVelocity = (((FVector(Voidgrid->LinearVelocity, 0)) - (((UKismetMathLibrary::GetDirectionUnitVector(Voidgrid->GetActorLocation(), FVector(Location, 0)))))) / (a)).Size();
				b = ((Voidgrid->LinearVelocity) - (Location)).Size();
			}
		}
	}
	else
	{
		if ((FVector(Voidgrid->LinearVelocity.GetSafeNormal(), 0) - (UKismetMathLibrary::GetDirectionUnitVector(Voidgrid->GetActorLocation(), FVector(Location, 0)))).Size() != 0)
		{
			return -1;

		}
	}
	float timeToLocation = (((-1 * b) + (sqrt((b * b) - (4 * a * c)))) / (2 * a));
	return (timetoVelocity + timeToLocation);
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
	float AngularDirection = ((Voidgrid->AngularVelocity) - (Velocity));
	UE_LOG(LogTemp, Warning, TEXT("AngularDirection = %f"), AngularDirection);
	return ((AngularDirection) / (GetMaximumAccelerationInDirection(AngularDirection)));

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