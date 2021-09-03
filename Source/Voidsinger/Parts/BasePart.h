// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/Parts/PartGridComponent.h"
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
	FIntPoint GetLocation();

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EPartRotation> GetRotation();

	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetShape();

	//UFUNCTION(BlueprintPure)
	//TArray<FIntPoint> GetMass();

	UFUNCTION(BlueprintCallable)
	void Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGrid);

protected:
	UPROPERTY(EditDefaultsOnly, NoClear)
	TArray<FIntPoint> DesiredShape;
	TArray<FIntPoint> DestroyedPixels;
	
	UPROPERTY(EditDefaultsOnly, NoClear)
	float Mass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;

	TEnumAsByte<EPartRotation> Rotation;

	FIntPoint Location;

	UPartGridComponent* PartGridComponent;

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	

	
};
