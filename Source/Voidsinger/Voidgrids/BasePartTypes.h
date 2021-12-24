// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BasePartTypes.generated.h"

class UBasePart;

//Stores the shape of a part
typedef TSet<GridLocationType> PartShapeType;

/**
 * Represents a rotation of a part.
 */
UENUM(BlueprintType)
enum class EPartRotation : uint8
{
	PR_0Degrees		UMETA(DisplayName = "0°"),
	PR_90Degrees	UMETA(DisplayName = "90°"),
	PR_180Degrees	UMETA(DisplayName = "180°"),
	PR_270Degrees	UMETA(DisplayName = "270°"),
};

/**
 * Handels Base Part Type operations.
 */
UCLASS()
class VOIDSINGER_API UBasePartTypesLibrary : public UObject
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Part Transform \/ */

public:
	/**
	 * Applies a given rotation to a given IntPoint.
	 *
	 * @param Target - The IntPoint to rotate.
	 * @param Rotation - The rotation to apply.
	 * @return The rotated IntPoint.
	 */
	UFUNCTION(BlueprintPure)
	static FIntPoint PartRotateIntPoint(FIntPoint Target, EPartRotation Rotation);

	/**
	 * Undoes a given rotation on a given IntPoint.
	 *
	 * @param Target - The IntPoint to rotate.
	 * @param Rotation - The rotation to Undos.
	 * @return The unrotated IntPoint.
	 */
	UFUNCTION(BlueprintPure)
	static FIntPoint PartUnRotateIntPoint(FIntPoint Target, EPartRotation Rotation);

	/* /\ Part Transform /\ *\
	\* -------------------- */
};

/**
 *  Stores the location and rotation of a part.
 */
USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartTransform
{
	GENERATED_BODY()

protected:
	//Stores the Location in IntPoint form for accessablity in blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Location;

public:
	//Stores the rotation of the part.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPartRotation Rotation;

	//Gets the location of the part.
	GridLocationType GetGridLocation()
	{
		return Location;
	}

	//Constucts a part transform with the given location and a part rotation.
	FPartTransform(GridLocationType Loc = GridLocationType(0, 0), EPartRotation Rot = EPartRotation::PR_0Degrees)
	{
		Location = Loc;
		Rotation = Rot;
	}

	//Constucts a part transform with the given location and a rotation that will be snaped to 90 degree intervals.
	FPartTransform(GridLocationType Loc, float Rot)
	{
		Location = Loc;
		Rotation = (EPartRotation)(((int32)Rot / 90) % 3);
	}

	GridLocationType TransformGridLocation(GridLocationType Target)
	{
		return UBasePartTypesLibrary::PartRotateIntPoint(Target, Rotation) + GetGridLocation();
	}

	PartShapeType TransformPartShape(PartShapeType Shape)
	{
		PartShapeType NewShape = PartShapeType();
		for (GridLocationType ShapeComponent : Shape)
		{
			NewShape.Emplace(TransformGridLocation(ShapeComponent));
		}

		return NewShape;
	}

	GridLocationType InverseTransformGridLocation(GridLocationType Target)
	{
		return UBasePartTypesLibrary::PartUnRotateIntPoint(Target - GetGridLocation(), Rotation);
	}

	PartShapeType InverseTransformPartShape(PartShapeType Shape)
	{
		PartShapeType NewShape = PartShapeType();
		for (GridLocationType ShapeComponent : Shape)
		{
			NewShape.Emplace(InverseTransformGridLocation(ShapeComponent));
		}

		return NewShape;
	}
};

/**
 * Stores all information required to replicate a part
 */
USTRUCT(BlueprintType)
struct FMinimalPartData
{
	GENERATED_BODY()

public:
	//Stores the class of the part
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TSubclassOf<UBasePart> Class;

	//Stores the location and rotation of the part
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FPartTransform Transform;

	//Stores the shape of the part
	PartShapeType Shape;

	FMinimalPartData(TSubclassOf<UBasePart> PartClass, FPartTransform PartTransform, PartShapeType PartShape)
	{
		Class = PartClass;
		Transform = PartTransform;
		Shape = PartShape;
	}
};

