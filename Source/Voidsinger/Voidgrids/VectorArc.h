// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VectorArc.generated.h"

/**
 * Stores an arc between two vectors.
 * Fast functions for arc matnimulation and testing.
 */
USTRUCT()
struct VOIDSINGER_API FVectorArc
{
	GENERATED_BODY()

	/**
	 * Initilizes this arc to be bound by the given vectors. If the limits are the same then the arc will contain the whole circle.
	 *
	 * @param LowerArcLimit - The lower limit of this arc. The arc will be in the clockwise dirction of this vector.
	 * @param UpperArcLimit - The upper limit of this arc. The arc will be in the counterclockwise dirction of this vector.
	 */
	FVectorArc(FVector2D LowerArcLimit = FVector2D::UnitVector, FVector2D UpperArcLimit = FVector2D::UnitVector)
	{
		SetArcBounds(LowerArcLimit, UpperArcLimit);
	}

	/**
	 * Attempts to shrink the arc to the given limits. If the limits srult in a wider arc then they will not be set.
	 * 
	 * @param LowerArcLimit - The target lower limit of this arc. The arc will be in the clockwise dirction of this vector.
	 * @param UpperArcLimit - The target upper limit of this arc. The arc will be in the counterclockwise dirction of this vector.
	 */
	void ShrinkArcBounds(FVector2D LowerArcLimit, FVector2D UpperArcLimit)
	{
		//If arc is not restricted then try inilizing the arc with the given limits
		if (!bArcRestricted)
		{
			SetArcBounds(LowerArcLimit, UpperArcLimit);
		}
		else
		{
			if (IsLocationInArc(LowerArcLimit))
			{
				LowerArcCotValue = GetVectorCot(LowerArcLimit);
				bLowerArcNegativeY = LowerArcLimit.Y < 0;
			}
			if (IsLocationInArc(UpperArcLimit))
			{
				UpperArcCotValue = GetVectorCot(UpperArcLimit);
				bUpperArcNegativeY = UpperArcLimit.Y < 0;
			}			
		}
	}

	/**
	 * Gets whether or not the given location is within the arc of this.
	 */
	bool IsLocationInArc(FVector2D Location, bool bInclusiveBounds = true)
	{
		if (!bArcRestricted)
		{
			return true;
		}
		float LocationCotValue = GetVectorCot(Location);

		if (bInclusiveBounds)
		{
			if (bLowerArcNegativeY == bUpperArcNegativeY)
			{
				if (LowerArcCotValue <= UpperArcCotValue)
				{
					return (LocationCotValue >= LowerArcCotValue && LocationCotValue <= UpperArcCotValue) && Location.Y < 0 == (bUpperArcNegativeY || bLowerArcNegativeY);
				}
				return LocationCotValue >= LowerArcCotValue || LocationCotValue <= UpperArcCotValue || Location.Y < 0 != (bUpperArcNegativeY || bLowerArcNegativeY);
			}

			return (bUpperArcNegativeY == Location.Y < 0) ? LocationCotValue <= UpperArcCotValue : LocationCotValue >= LowerArcCotValue;
		}

		if (bLowerArcNegativeY == bUpperArcNegativeY)
		{
			if (LowerArcCotValue <= UpperArcCotValue)
			{
				return (LocationCotValue > LowerArcCotValue && LocationCotValue < UpperArcCotValue) && Location.Y < 0 == (bUpperArcNegativeY || bLowerArcNegativeY);
			}
			return LocationCotValue > LowerArcCotValue || LocationCotValue < UpperArcCotValue || Location.Y < 0 != (bUpperArcNegativeY || bLowerArcNegativeY);
		}

		return (bUpperArcNegativeY == Location.Y < 0) ? LocationCotValue < UpperArcCotValue : LocationCotValue > LowerArcCotValue;
	}

	/**
	 * Gets whether or not the given location is within the arc of this.
	 */
	bool DoesLinePassThoughArc(FVector2D LineStart, FVector2D LineEnd, bool bInclusiveBounds = true)
	{
		FVector2D UpperArcBoundLocation = FVector2D(UpperArcCotValue, 1);
		if (bUpperArcNegativeY)
		{
			UpperArcBoundLocation *= -1;
		}
		FVector2D LowerArcBoundLocation = FVector2D(LowerArcCotValue, 1);
		if (bLowerArcNegativeY)
		{
			LowerArcBoundLocation *= -1;
		}

		//     | ----------------------- Get whether line enpoints are in bounds ---------------------- |    | --------------------------------------------------------------------------------- Get whether line crosses bounds ---------------------------------------------------------------------------------- |                                                                               
		return IsLocationInArc(LineStart, bInclusiveBounds) || IsLocationInArc(LineEnd, bInclusiveBounds) || FVectorArc(LineStart, LineEnd).IsLocationInArc(UpperArcBoundLocation, bInclusiveBounds) || (!bInclusiveBounds && FVectorArc(LineStart, LineEnd).IsLocationInArc(LowerArcBoundLocation, bInclusiveBounds));
	}

	/**
	 * Sets this arc to be bound by the given vectors. If the limits are the same then the arc will contain the whole circle.
	 * 
	 * @param LowerArcLimit - The lower limit of this arc. The arc will be in the clockwise dirction of this vector.
	 * @param UpperArcLimit - The upper limit of this arc. The arc will be in the counterclockwise dirction of this vector.
	 */
	void SetArcBounds(FVector2D LowerArcLimit, FVector2D UpperArcLimit)
	{
		bArcRestricted = LowerArcLimit != UpperArcLimit;
		if (bArcRestricted)
		{
			LowerArcCotValue = GetVectorCot(LowerArcLimit);
			bLowerArcNegativeY = LowerArcLimit.Y < 0;
			UpperArcCotValue = GetVectorCot(UpperArcLimit);
			bUpperArcNegativeY = UpperArcLimit.Y < 0;
		}
	}

//private:
	/**
	 * Gets the Cotangent of the vector. This is a safe operation, will return FLT_MAX if Y = 0.
	 * 
	 * @param Vector - The vector to get the cotangent of.
	 * @return The cotangent of Vector.
	 */
	float GetVectorCot(FVector2D Vector)
	{
		return Vector.Y != 0 ? Vector.X / Vector.Y : FMath::Sign(Vector.X) * FLT_MAX;
	}

	//The lower arc vetor's X/Y value.
	UPROPERTY()
	float LowerArcCotValue;

	//Whether or not the lower arc vetor's Y value is negative.
	UPROPERTY()
	bool bLowerArcNegativeY;

	//The upper arc vetor's X/Y value.
	UPROPERTY()
	float UpperArcCotValue;

	//Whether or not the upper arc vetor's Y value is negative.
	UPROPERTY()
	bool bUpperArcNegativeY;

	//Whether or not this arc is bound by two differnt vectors.
	UPROPERTY()
	bool bArcRestricted;
};