// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Voidsinger/FunctionLibrary.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseResourceSystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VOIDSINGER_API UBaseResourceSystem : public UObject
{
	GENERATED_BODY()
	

public:
	UBaseResourceSystem();

	UFUNCTION(BlueprintCallable)
	void AddResources(float Amount);

	UFUNCTION(BlueprintCallable)
	void DrawResources(float Amount);

	UFUNCTION(BlueprintCallable)
	void AddPart(UBasePart* AddedPart);

	UFUNCTION(BlueprintCallable)
	void RemovePart(UBasePart* RemovedPart);

	UFUNCTION(BlueprintCallable)
	void RemovePixel(FIntPoint Pixel);

	UFUNCTION(BlueprintCallable)
	void MergeSystems(UBaseResourceSystem* MergedSystem);

	UFUNCTION(BlueprintCallable)
	void CreateNewSystem(TArray<UBasePart*> RemovedParts);

	UFUNCTION(BlueprintCallable)
	void AddSection(TArray<UBasePart*> AddedParts);

	UFUNCTION(BlueprintCallable)
	void RemoveSection(TArray<UBasePart*> RemovedParts);

	UFUNCTION()
	TArray<FIntPoint> FindConnectedShape(TArray<UBasePart*> Parts, TArray<FIntPoint> Shape);

	UFUNCTION(BlueprintCallable)
	bool AreShapesAdjacent(TArray<FIntPoint> Shape1, TArray<FIntPoint> Shape2);

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EResourceType> GetType();

	UFUNCTION(BlueprintCallable)
	void SetType(TEnumAsByte<EResourceType> Type);

	virtual class UWorld* GetWorld() const override;

	UFUNCTION()
	TMap<FIntPoint, FPartData> GetMapFromConnectedParts();

protected:

	UPROPERTY()
	float ResourceAmount;

	UPROPERTY()
	TArray<UBasePart*> ConnectedParts;
	
	UPROPERTY()
	TArray<FUBasePartArray> SeparatedSystems;

	UPROPERTY()
	TEnumAsByte<EResourceType> SystemType;

};
