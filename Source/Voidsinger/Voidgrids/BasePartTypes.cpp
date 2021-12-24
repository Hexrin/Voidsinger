// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePartTypes.h"

/**
 * Applies a given rotation to a given IntPoint.
 *
 * @param Target - The IntPoint to rotate.
 * @param Rotation - The rotation to apply.
 * @return The rotated IntPoint.
 */
UFUNCTION(BlueprintPure)
FIntPoint UBasePartTypesLibrary::PartRotateIntPoint(FIntPoint Target, EPartRotation Rotation)
{
	switch (Rotation)
	{
	case EPartRotation::PR_0Degrees:
		return Target;
	case EPartRotation::PR_90Degrees:
		return FIntPoint(-Target.Y, Target.X);
	case EPartRotation::PR_180Degrees:
		return Target * -1;
	case EPartRotation::PR_270Degrees:
		return FIntPoint(Target.Y, -Target.X);
	default:
		UE_LOG(LogTemp, Error, TEXT("PartRotateIntPoint invalid Rotation"));
		return Target;
	}
}

/**
 * Undoes a given rotation on a given IntPoint.
 *
 * @param Target - The IntPoint to unrotate.
 * @param Rotation - The rotation to undo.
 * @return The unrotated IntPoint.
 */
UFUNCTION(BlueprintPure)
FIntPoint UBasePartTypesLibrary::PartUnRotateIntPoint(FIntPoint Target, EPartRotation Rotation)
{
	switch (Rotation)
	{
	case EPartRotation::PR_0Degrees:
		return Target;
	case EPartRotation::PR_90Degrees:
		return FIntPoint(Target.Y, -Target.X);
	case EPartRotation::PR_180Degrees:
		return Target * -1;
	case EPartRotation::PR_270Degrees:
		return FIntPoint(-Target.Y, Target.X);
	default:
		UE_LOG(LogTemp, Error, TEXT("PartRotateIntPoint invalid Rotation"));
		return Target;
	}
}