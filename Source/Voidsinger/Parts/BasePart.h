// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/FunctionLibrary.h"
#include "Voidsinger/Parts/PartGridComponent.h"
#include "BasePart.generated.h"

/**
 * 
 */

class UBaseResourceSystem;
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
	const FIntPoint GetLocation();

	UFUNCTION(BlueprintPure)
	const TEnumAsByte<EPartRotation> GetRotation();

	UFUNCTION(BlueprintPure)
	const TArray<FIntPoint> GetShape();

	UFUNCTION(BlueprintPure)
	float GetMass();

	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> GetResourceTypes();

	UFUNCTION(BlueprintCallable)
	void Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGridComp);

	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TEnumAsByte<EResourceType> ResourceType);

	UPROPERTY(EditAnywhere)
	UStaticMesh* PixelMesh;

protected:
	UPROPERTY(EditDefaultsOnly, NoClear)
	TArray<FIntPoint> DesiredShape;

	UPROPERTY(EditDefaultsOnly, NoClear)
	float Mass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;


private:
	TArray<FIntPoint> DestroyedPixels;
	
	TEnumAsByte<EPartRotation> Rotation;

	FIntPoint Location;

	TArray<UBaseResourceSystem*
	
	UPartGridComponent* PartGridComponent;

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	float _Mass;
	int _Cost;
	TArray<FIntPoint> _DesiredShape;

	
};
