// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoidsingerTypes.h"
#include "DrawDebugHelpers.h"
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

	//Used for the creation widget. Avoid using this at runtime, it's quite a performance hit. 
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Child Classes", Keywords = "Get Child Classes"), Category = Class)
	static TArray<UClass*> GetClasses(UClass* ParentClass);

	UFUNCTION(BlueprintCallable)
	static bool SetActorTransformSweepComponets(AActor* Target, FHitResult & Hit, TArray <UPrimitiveComponent*> PrimComps, const FTransform& NewTransform);

	//Explode at a global location, affecting all part grids in the radius
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void ExplodeAtWorldLocation(const UObject* WorldContextObject, FVector WorldLocation, float ExplosionRadius);

	//A recursive function that will check the shape it's provided with for any parts that are not connected to each other
	UFUNCTION()
	static TArray<FIntPoint> FindConnectedShape(TArray<FIntPoint> Shape, TMap<FIntPoint, FPartData> ConnectedPartsMap, bool CheckFunctionality = false);

	//This will return a bit based on the location given. The location given should be relative to the pixel in question.
	//So -1, -1 will be the bottom left, or 16.
	UFUNCTION()
	static int GetBitNumberFromLocation(FIntPoint Location);
};
