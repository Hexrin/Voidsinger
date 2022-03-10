// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidgridBlueprintUtilityLibrary.h"

/**
 * Applies a given rotation to a given IntPoint.
 *
 * @param GridLocation - The IntPoint to rotate.
 * @param Rotation - The rotation to apply.
 * @return The rotated IntPoint.
 */
FIntPoint UVoidgridBlueprintUtilityLibrary::RotateGridLocation(FIntPoint GridLocation, EPartRotation Rotation)
{
	return UPartRotationFunctions::RotateGridLocation(GridLocation, Rotation);
}

/**
 * Undoes a given rotation on a given IntPoint.
 *
 * @param GridLocation - The IntPoint to rotate.
 * @param Rotation - The rotation to Undoes.
 * @return The unrotated IntPoint.
 */
FIntPoint UVoidgridBlueprintUtilityLibrary::UnrotateGridLocation(FIntPoint GridLocation, EPartRotation Rotation)
{
	return UPartRotationFunctions::UnrotateGridLocation(GridLocation, Rotation);
}

/**
 * Applies a given transform to a given IntPoint.
 *
 * @param GridLocation - The IntPoint to transform.
 * @param Transform - The transform to apply.
 * @return The rotated IntPoint.
 */
FIntPoint UVoidgridBlueprintUtilityLibrary::TransformGridLocation(FIntPoint GridLocation, FPartTransform Transform)
{
	return Transform.TransformGridLocation(GridLocation);
}

/**
 * Undoes a given transform on a given IntPoint.
 *
 * @param GridLocation - The IntPoint to transform.
 * @param Transform - The transform to undo.
 * @return The unrotated IntPoint.
 */
FIntPoint UVoidgridBlueprintUtilityLibrary::InverseTransformGridLocation(FIntPoint GridLocation, FPartTransform Transform)
{
	return Transform.InverseTransformGridLocation(GridLocation);
}

/* /\ Part Transform /\ *\
\* -------------------- */