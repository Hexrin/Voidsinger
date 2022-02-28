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

	FExplosionArc(FVector2D LowerArcLimit = FVector2D::UnitVector, FVector2D UpperArcLimit = FVector2D::UnitVector)
	{
		SetNewArcValue(LowerArcLimit, UpperArcLimit);
	}

	void SetNewArcValue(FVector2D LowerArcLimit, FVector2D UpperArcLimit)
	{
		//bLowerArcNegativeY = LowerArcLimit.Y < 0;
		//LowerArcXValue = bLowerArcNegativeY ? FMath::Min(LowerArcXValue, LowerArcLimit.X) : FMath::Max(LowerArcXValue, LowerArcLimit.X)


		//bUpperArcNegativeY = UpperArcLimit.Y < 0;
		//UpperArcXValue = bUpperArcNegativeY ? FMath::Min(UpperArcXValue, UpperArcLimit.X) : FMath::Max(UpperArcXValue, UpperArcLimit.X)
	}

	/**
	 * Gets whether or not the given location is within the arc of this
	 */
	bool IsLocationInArc(FVector2D Location)
	{
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
};