// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoidgridBlueprintUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UVoidgridBlueprintUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Part Transform \/ */

public:

	/**
	 * Applies a given rotation to a given IntPoint.
	 *
	 * @param GridLocation - The IntPoint to rotate.
	 * @param Rotation - The rotation to apply.
	 * @return The rotated IntPoint.
	 */
	UFUNCTION(BlueprintPure)
	static FIntPoint RotateGridLocation(FIntPoint GridLocation, EPartRotation Rotation);

	/**
	 * Undoes a given rotation on a given IntPoint.
	 *
	 * @param GridLocation - The IntPoint to rotate.
	 * @param Rotation - The rotation to Undos.
	 * @return The unrotated IntPoint.
	 */
	UFUNCTION(BlueprintPure)
	static FIntPoint UnrotateGridLocation(FIntPoint GridLocation, EPartRotation Rotation);

	/**
	 * Applies a given transform to a given IntPoint.
	 *
	 * @param GridLocation - The IntPoint to transform.
	 * @param Transform - The transform to apply.
	 * @return The rotated IntPoint.
	 */
	UFUNCTION(BlueprintPure)
	static FIntPoint TransformGridLocation(FIntPoint GridLocation, FPartTransform Transform);

	/**
	 * Undoes a given transform on a given IntPoint.
	 *
	 * @param GridLocation - The IntPoint to transform.
	 * @param Transform - The transform to undo.
	 * @return The unrotated IntPoint.
	 */
	UFUNCTION(BlueprintPure)
	static FIntPoint InverseTransformGridLocation(FIntPoint GridLocation, FPartTransform Transform);

	/* /\ Part Transform /\ *\
	\* -------------------- */

	/**
	 * Gets the default shape of this part.
	 *
	 * @return An array of all the locations of the pixels in the default shape.
	 */
	UFUNCTION(BlueprintPure)
	static TArray<FIntPoint> GetDefaultShapeArray(TSubclassOf<UBasePart> PartClass);
};
