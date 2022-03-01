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
	void ShrinkArcLimits(FVector2D LowerArcLimit, FVector2D UpperArcLimit)
	{
		//If arc is not restricted then try inilizing the arc with the given limits
		if (!bArcRestricted)
		{
			SetArcBounds(LowerArcLimit, UpperArcLimit);
		}
		else
		{
			//The X/Y of the LowerArcLimit.
			float LowerArcLimitCot = GetVectorCot(LowerArcLimit);

			//The X/Y of the UpperArcLimit.
			float UpperArcLimitCot = GetVectorCot(UpperArcLimit);

			//If the limits are in the same hemisphere
			if (bUpperArcNegativeY == bLowerArcNegativeY)
			{
				//
				if (LowerArcLimit.Y < 0 == bLowerArcNegativeY)
				{
					LowerArcCotValue = LowerArcLimitCot < UpperArcLimitCot ? FMath::Clamp(LowerArcCotValue, LowerArcLimitCot, UpperArcCotValue) : FMath::Clamp(UpperArcCotValue, UpperArcCotValue, LowerArcLimitCot);
				}
				if (UpperArcLimit.Y < 0 == bUpperArcNegativeY)
				{
					UpperArcCotValue = UpperArcLimitCot < LowerArcLimitCot ? FMath::Clamp(UpperArcCotValue, UpperArcLimitCot, LowerArcCotValue) : FMath::Clamp(UpperArcCotValue, LowerArcCotValue, UpperArcLimitCot);
				}
			}
			else
			{
				if (LowerArcLimit.Y < 0 == bLowerArcNegativeY)
				{
					LowerArcCotValue = bLowerArcNegativeY ? FMath::Clamp(LowerArcCotValue, LowerArcLimitCot, UpperArcCotValue) : FMath::Clamp(UpperArcCotValue, UpperArcCotValue, LowerArcLimitCot);
				}
				else
				{
					LowerArcCotValue = LowerArcLimitCot;
					bLowerArcNegativeY = LowerArcLimit.Y < 0;
				}
				if (UpperArcLimit.Y < 0 == bUpperArcNegativeY)
				{
					UpperArcCotValue = bUpperArcNegativeY ? FMath::Clamp(UpperArcCotValue, UpperArcLimitCot, LowerArcCotValue) : FMath::Clamp(UpperArcCotValue, LowerArcCotValue, UpperArcLimitCot);
				}
				else
				{
					UpperArcCotValue = UpperArcLimitCot;
					bUpperArcNegativeY = UpperArcLimit.Y < 0;
				}
			}
		}
	}

	/**
	 * Gets whether or not the given location is within the arc of this.
	 */
	bool IsLocationInArc(FVector2D Location)
	{
		if (!bArcRestricted)
		{
			return true;
		}
		float LocationCotValue = GetVectorCot(Location);

		//if (UpperArcCotValue == 0 && LowerArcCotValue == 0)
		//{
		//	return Location.X == 0 || bUpperArcNegativeY == Location.X < 0;
		//}


		if (bLowerArcNegativeY == bUpperArcNegativeY)
		{
			//                 true                     true
			if (UpperArcCotValue > LowerArcCotValue == bUpperArcNegativeY)
			{
				return FMath::IsWithinInclusive(LocationCotValue, LowerArcCotValue, UpperArcCotValue) && Location.Y < 0 == (bUpperArcNegativeY || bLowerArcNegativeY);
			}
			return LocationCotValue >= LowerArcCotValue || LocationCotValue <= UpperArcCotValue || Location.Y < 0 != (bUpperArcNegativeY || bLowerArcNegativeY);
		}
		//Loc = (-1,0)
		//          false                false               -inf                 1                      -inf                 0
		return (bUpperArcNegativeY == Location.Y < 0) ? LocationCotValue <= UpperArcCotValue : LocationCotValue >= LowerArcCotValue;
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

private:
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