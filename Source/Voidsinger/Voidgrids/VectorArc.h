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

	//The lower arc vetor's X value.
	UPROPERTY()
	float LowerArcXValue;

	//Whether or not the lower arc vetor's Y value is negative.
	UPROPERTY()
	bool bLowerArcNegativeY;

	//The upper arc vetor's X value.
	UPROPERTY()
	float UpperArcXValue;

	//Whether or not the upper arc vetor's Y value is negative.
	UPROPERTY()
	bool bUpperArcNegativeY;

	FVectorArc(FVector2D LowerArcLimit = FVector2D::UnitVector, FVector2D UpperArcLimit = FVector2D::UnitVector)
	{
		LowerArcXValue = LowerArcLimit.X;
		bLowerArcNegativeY = LowerArcLimit.Y < 0;
		UpperArcXValue = UpperArcLimit.X;
		bUpperArcNegativeY = UpperArcLimit.Y < 0;
		bArcRestricted = LowerArcLimit == UpperArcLimit
	}

	void SetNewArcValue(FVector2D LowerArcLimit, FVector2D UpperArcLimit)
	{
		//If arc is not
		if (UpperArcXValue == LowerArcXValue && bLowerArcNegativeY == bUpperArcNegativeY)
		{
			LowerArcXValue = LowerArcLimit.X;
			bLowerArcNegativeY = LowerArcLimit.Y < 0;
			UpperArcXValue = UpperArcLimit.X;
			bUpperArcNegativeY = UpperArcLimit.Y < 0;
		}
		else
		{
			if (bUpperArcNegativeY == bLowerArcNegativeY)
			{
				if (LowerArcLimit.Y < 0 == bLowerArcNegativeY)
				{
					LowerArcXValue = LowerArcLimit.X < UpperArcLimit.X ? FMath::Min(LowerArcXValue, LowerArcLimit.X) : FMath::Max(LowerArcXValue, LowerArcLimit.X);
				}
				if (UpperArcLimit.Y < 0 == bUpperArcNegativeY)
				{
					UpperArcXValue = UpperArcLimit.X < LowerArcLimit.X ? FMath::Min(UpperArcXValue, UpperArcLimit.X) : FMath::Max(UpperArcXValue, UpperArcLimit.X);
				}
			}
			else
			{
				if (LowerArcLimit.Y < 0 == bLowerArcNegativeY)
				{
					LowerArcXValue = bLowerArcNegativeY ? FMath::Max(LowerArcXValue, LowerArcLimit.X) : FMath::Min(LowerArcXValue, LowerArcLimit.X);
				}
				else
				{
					LowerArcXValue = LowerArcLimit.X;
					bLowerArcNegativeY = LowerArcLimit.Y < 0;
				}
				if (UpperArcLimit.Y < 0 == bUpperArcNegativeY)
				{
					UpperArcXValue = bUpperArcNegativeY ? FMath::Max(UpperArcXValue, UpperArcLimit.X) : FMath::Min(UpperArcXValue, UpperArcLimit.X);
				}
				else
				{
					UpperArcXValue = UpperArcLimit.X;
					bUpperArcNegativeY = UpperArcLimit.Y < 0;
				}
			}
		}
	}

	/**
	 * Gets whether or not the given location is within the arc of this
	 */
	bool IsLocationInArc(FVector2D Location)
	{
		if (UpperArcXValue == LowerArcXValue && bLowerArcNegativeY == bUpperArcNegativeY)
		{
			return true;
		}

		if (bLowerArcNegativeY == bUpperArcNegativeY && (Location.Y < 0 == bUpperArcNegativeY))
		{
			if (UpperArcXValue < LowerArcXValue)
			{
				return FMath::IsWithinInclusive(Location.X, UpperArcXValue, LowerArcXValue);
			}
			return Location.X >= UpperArcXValue || Location.X <= LowerArcXValue;
		}
		if (bUpperArcNegativeY)
		{
			return Location.Y < 0 ? Location.X <= UpperArcXValue : Location.X <= LowerArcXValue;
		}
		else
		{
			return Location.Y < 0 ? Location.X >= LowerArcXValue : Location.X >= UpperArcXValue;
		}
	}

private:
	UPROPERTY()
	bool bArcRestricted;
};