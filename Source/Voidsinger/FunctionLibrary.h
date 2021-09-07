// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoidsingerTypes.h"
#include "FunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	static FIntPoint RotateIntPoint(FIntPoint IntPoint, TEnumAsByte<EPartRotation> Rotation);

	UFUNCTION(BlueprintPure)
	static bool PointsConected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint);
	static bool PointsConected(TMap<FIntPoint, UBasePart*> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConectivityArray);
};
