// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VectorArc.generated.h"

/**
 * Stores an arc bound by two vectors.
 * Fast functions for arc manipulation and testing.
 */
USTRUCT()
struct VOIDSINGER_API FVectorArc
{
	GENERATED_BODY()

	/**
	 * Initilizes this arc to be bound by the given vectors. If the limits are the same then the arc will contain the whole circle.
	 *
	 * @param LowerArcBound - The lower bound of this arc. The arc will be in the clockwise dirction of this vector.
	 * @param UpperArcBound - The upper bound of this arc. The arc will be in the counterclockwise dirction of this vector.
	 */
	FVectorArc(FVector2D LowerArcBound = FVector2D::UnitVector, FVector2D UpperArcBound = FVector2D::UnitVector)
	{
		SetArcBounds(LowerArcBound, UpperArcBound);
	}

	/**
	 * Attempts to shrink the arc to the given bounds. If a bound would result in a wider arc then the .
	 * 
	 * @param LowerArcBound - The target lower bound of this arc. The arc will be in the clockwise dirction of this vector.
	 * @param LowerArcBound - The target upper bound of this arc. The arc will be in the counterclockwise dirction of this vector.
	 */
	void ShrinkArcBounds(FVector2D LowerArcBound, FVector2D UpperArcBound)
	{
		//If arc is not restricted then inilize the arc with the given limits.
		if (!bArcBounded)
		{
			SetArcBounds(LowerArcBound, UpperArcBound);
		}
		else
		{
			//Adjust lower bound
			if (IsLocationInArc(LowerArcBound))
			{
				LowerBoundCotValue = GetVectorCot(LowerArcBound);
				bLowerBoundNegativeY = LowerArcBound.Y < 0;
			}

			//Adjust upper bound
			if (IsLocationInArc(UpperArcBound))
			{
				UpperBoundCotValue = GetVectorCot(UpperArcBound);
				bUpperBoundNegativeY = UpperArcBound.Y < 0;
			}			
		}
	}

	/**
	 * Gets whether or not the given location is between the LowerBound and UpperBound of this.
	 * 
	 * @param Location - The location to check.
	 * @param bInclusiveBounds - Whether or not to count a location on the bounds as between them.
	 * @return Whether or not the given location is between the LowerBound and UpperBound of this.
	 */
	bool IsLocationInArc(FVector2D Location, bool bInclusiveBounds = true) const
	{
		//If arc not bounded the location will always be in it.
		if (!bArcBounded)
		{
			return true;
		}

		//The cotangent value of Location.
		float LocationCotValue = GetVectorCot(Location);

		//If bounds are in the same hemisphere
		if (bLowerBoundNegativeY == bUpperBoundNegativeY)
		{
			//If do not include other hemisphere
			if (LowerBoundCotValue <= UpperBoundCotValue)
			{
				//     | --------------------------------------------------- Is Location between bounds ---------------------------------------------------- |    | ------------ Is Location in same hemisphere ------------ |
				return IsGreaterThan(LocationCotValue, LowerBoundCotValue, bInclusiveBounds) && IsLessThan(LocationCotValue, UpperBoundCotValue, bInclusiveBounds) && Location.Y < 0 == (bUpperBoundNegativeY || bLowerBoundNegativeY);
			}
			//     | ------------------------------------------------ Is Location *NOT* between bounds ------------------------------------------------- |    | --------- Is Location *NOT* in same hemisphere --------- |
			return IsGreaterThan(LocationCotValue, LowerBoundCotValue, bInclusiveBounds) || IsLessThan(LocationCotValue, UpperBoundCotValue, bInclusiveBounds) || Location.Y < 0 != (bUpperBoundNegativeY || bLowerBoundNegativeY);
		}

		//Based on location hemisphere check to see if location is in the direction of the other bound.
		return (bUpperBoundNegativeY == Location.Y < 0) ? IsLessThan(LocationCotValue, UpperBoundCotValue, bInclusiveBounds) : IsGreaterThan(LocationCotValue, LowerBoundCotValue, bInclusiveBounds);
	}

	/**
	 * Gets whether or not the line is passes through the arc of this.
	 *
	 * @param LineStart - One endpoint of the line to check.
	 * @param LineEnd - One endpoint of the line to check.
	 * @param bInclusiveBounds - Whether or not to count a location on the bounds as between them.
	 * @return Whether or not the line is passes through the arc of this.
	 */
	bool DoesLinePassThoughArc(FVector2D LineStart, FVector2D LineEnd, bool bInclusiveBounds = true) const
	{
		//     | ----------------------- Get whether line enpoints are in bounds ---------------------- |    | --------------------------------------------------------------------------- Get whether line crosses bounds ----------------------------------------------------------------------------- |                                                                               
		return IsLocationInArc(LineStart, bInclusiveBounds) || IsLocationInArc(LineEnd, bInclusiveBounds) || FVectorArc(LineStart, LineEnd).IsLocationInArc(GetUpperBound(), bInclusiveBounds) || (!bInclusiveBounds && FVectorArc(LineStart, LineEnd).IsLocationInArc(GetLowerBound(), bInclusiveBounds));
	}

	/**
	 * Sets this arc to be bound by the given vectors. If the limits are the same then the arc will contain the whole circle.
	 * 
	 * @param LowerArcLimit - The lower limit of this arc. The arc will be in the clockwise dirction of this vector.
	 * @param UpperArcLimit - The upper limit of this arc. The arc will be in the counterclockwise dirction of this vector.
	 */
	void SetArcBounds(FVector2D LowerArcBound, FVector2D UpperArcBound)
	{
		bArcBounded = LowerArcBound != UpperArcBound;
		if (bArcBounded)
		{
			LowerBoundCotValue = GetVectorCot(LowerArcBound);
			bLowerBoundNegativeY = LowerArcBound.Y < 0;
			UpperBoundCotValue = GetVectorCot(UpperArcBound);
			bUpperBoundNegativeY = UpperArcBound.Y < 0;
		}
	}

	/**
	 * Gets the upper bound of this arc as a vector.
	 * 
	 * @return The upper bound of this arc as a vector.
	 */
	FVector2D GetUpperBound() const
	{
		FVector2D UpperArcBoundLocation = FVector2D(UpperBoundCotValue, 1);
		if (bUpperBoundNegativeY)
		{
			UpperArcBoundLocation *= -1;
		}
		return UpperArcBoundLocation;
	}

	/**
	 * Gets the lower bound of this arc as a vector.
	 * 
	 * @return The lower bound of this arc as a vector.
	 */
	FVector2D GetLowerBound() const
	{
		FVector2D LowerArcBoundLocation = FVector2D(LowerBoundCotValue, 1);
		if (bLowerBoundNegativeY)
		{
			LowerArcBoundLocation *= -1;
		}
		return LowerArcBoundLocation;
	}

private:
	/**
	 * Gets the cotangent of the vector. This is a safe operation, will return FLT_MAX if Y = 0.
	 * 
	 * @param Vector - The vector to get the cotangent of.
	 * @return The cotangent of Vector.
	 */
	float GetVectorCot(FVector2D Vector) const
	{
		return Vector.Y != 0 ? Vector.X / Vector.Y : FMath::Sign(Vector.X) * FLT_MAX;
	}

	/**
	 * Gets whether or not A < B.
	 * 
	 * @param A - The value to test for lesserness. 
     * @param B - The value to test for greaterness. 
	 * @param bInclusive - Whether or not to count equality as less than.
	 * @return Whether or not A < B.
	 */
	bool IsLessThan(float A, float B, bool bInclusive) const
	{
		return bInclusive ? A < B : A <= B;
	}

	/**
	 * Gets whether or not A > B.
	 *
	 * @param A - The value to test for greaterness.
	 * @param B - The value to test for lesserness.
	 * @param bInclusive - Whether or not to count equality as greater than.
	 * @return Whether or not A > B.
	 */
	bool IsGreaterThan(float A, float B, bool bInclusive) const
	{
		return bInclusive ? A > B : A >= B;
	}

	//The lower arc bound's cotangent (X/Y) value.
	UPROPERTY()
	float LowerBoundCotValue;

	//Whether or not the lower arc vetor's Y value is negative.
	UPROPERTY()
	bool bLowerBoundNegativeY;

	//The upper arc bound's cotangent (X/Y) value.
	UPROPERTY()
	float UpperBoundCotValue;

	//Whether or not the upper arc vetor's Y value is negative.
	UPROPERTY()
	bool bUpperBoundNegativeY;

	//Whether or not this arc is bound by two differnt vectors.
	UPROPERTY()
	bool bArcBounded;
};