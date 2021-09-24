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

	UFUNCTION(BlueprintPure)
	static bool PointsConnected(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint);
	static bool PointsConnected(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray);

	UFUNCTION(BlueprintPure)
	static bool PointsConnectedWithFunctionality(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint);
	static bool PointsConnectedWithFunctionality(TMap<FIntPoint, FPartData> PartGrid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray);

	UFUNCTION(BlueprintPure)
	static FVector SafeDivide(FVector V1, FVector V2);

	/*UFUNCTION(BlueprintCallable)
	static bool SetActorLocationSweepComponets
	(
		AActor* Target,
		FHitResult & Hits,
		TArray <UPrimitiveComponent*> PrimComps,
		const FTransform& Start,
		const FTransform& End,
		const FComponentQueryParams& Params
	);*/
};
