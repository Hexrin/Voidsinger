// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseResourceSystem.generated.h"

/**
 * 
 */
UCLASS()
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
	void MergeSystems(UBaseResourceSystem* MergedSystem);

	UFUNCTION(BlueprintCallable)
	void RemoveSection(TArray<UBasePart*> RemovedParts);

	UFUNCTION(BlueprintCallable)
	void AddSection(TArray<UBasePart*> AddedParts);

	UFUNCTION(BlueprintCallable)
	void ScanSystemForBreaks();

	UFUNCTION(BlueprintCallable)
	bool IsPartAdjacent(UBasePart* Part);

private:

	UPROPERTY()
	float ResourceAmount;

	UPROPERTY()
	TArray<UBasePart*> ConnectedParts;

};
