// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/FunctionLibrary.h"
#include "Voidsinger/Ships/BaseShip.h"
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

	UFUNCTION()
	void DestroyPixel(FIntPoint RelativeLoc);

	UFUNCTION(BlueprintPure)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> GetResourceTypes();

	UFUNCTION()
	void Init(FIntPoint Loc, TEnumAsByte<EPartRotation> Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType);

	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TEnumAsByte<EResourceType> ResourceType);

	UFUNCTION(BlueprintPure)
	TArray<UBaseResourceSystem*> GetSystems();

	UFUNCTION(BlueprintPure)
	UBaseResourceSystem* GetSystemByType(TEnumAsByte<EResourceType> Type);
	
	UFUNCTION(BlueprintPure)
	const UPartGridComponent* GetPartGrid();

	UFUNCTION(BlueprintCallable)
	void AddToSystem(UBaseResourceSystem* System);

	UPROPERTY(EditAnywhere)
	UStaticMesh* PixelMesh;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FIntPoint> DesiredShape;

	UPROPERTY(EditDefaultsOnly, NoClear)
	float Mass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Cost;


private:

	UPROPERTY()
	TArray<FIntPoint> ActualShape;

	UPROPERTY()
	TArray<FIntPoint> RotatedShape;

	UPROPERTY()
	FArrayBounds Bounds;

	UPROPERTY()
	TEnumAsByte<EPartRotation> Rotation;

	UPROPERTY()
	FIntPoint Location;

	UPROPERTY()
	TArray<UBaseResourceSystem*> Systems;
	
	UPROPERTY()
	UPartGridComponent* PartGridComponent;

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EResourceType>, FIntPointArray> ResourceTypes;

	
};
