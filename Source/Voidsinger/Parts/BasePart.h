// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "BasePart.generated.h"

/**
 * 
 */

enum EPartRotation;

UCLASS(BlueprintType, Blueprintable)
class VOIDSINGER_API UBasePart : public UObject
{

	GENERATED_BODY()

public:
	UBasePart();



	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetDesiredShape();
	const TArray<FIntPoint> GetDesiredShape(TEnumAsByte<EPartRotation> Rot);

	UFUNCTION(BlueprintPure)
	const FArrayBounds GetShapeBounds();
	const FArrayBounds GetShapeBounds(TEnumAsByte<EPartRotation> Rot);

	UFUNCTION(BlueprintPure)
	FIntPoint GetPartLocation();

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EPartRotation> GetPartRotation();

	
	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetShape();

	TEnumAsByte<EPartRotation> Rotation;
protected:
	UPROPERTY(EditDefaultsOnly, NoClear)
	TArray<FIntPoint> DesiredShape;

	

	
};
