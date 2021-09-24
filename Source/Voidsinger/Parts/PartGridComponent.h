// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Voidsinger/SaveShip.h"
#include "DrawDebugHelpers.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "PartGridComponent.generated.h"

class UBasePart;
class UBaseThrusterPart;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class VOIDSINGER_API UPartGridComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPartGridComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//---Ship Creation---

	UFUNCTION(BlueprintCallable)
	bool AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace = false);
	bool AddPart(TArray<FIntPoint> PartialPartShape, TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace = false);

	UFUNCTION(BlueprintCallable)
	bool RemovePart(FIntPoint Location);



	//---Destruction---

	UFUNCTION(BlueprintCallable)
	bool DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart);
	bool DestroyPixel(FIntPoint Location);

	UFUNCTION(BlueprintCallable)
	void ApplyHeatAtLocation(FVector WorldLocation, float HeatToApply = 1);
	void ApplyHeatAtLocation(FIntPoint RelativeLocation, float HeatToApply = 1);

	//Explodes at the given location with the given radius.
	UFUNCTION(BlueprintCallable)
	void ExplodeAtLocation(FVector WorldLocation, float ExplosionRadius);

	//Checks if a 2d Rectangle contains the location given
	UFUNCTION()
	bool BoxContainsLocation(FVector2D TopLeft, FVector2D BottomRight, FVector2D Location);

	//Returns the quadrant the location is in with respect to the origin. If the location is on an axis, then it will return 5 for positive Y, 6 for positive X, 7 for negative Y, and 8 for negative X. Remember that unreal is left-handed, so X is vertical and Y is horizontal. Returns 0 if the Location and origin are equal.
	UFUNCTION()
	int GetQuadrantFromLocation(FVector2D Location, FVector2D Origin);

	//Returns true if the line intersects the box.
	UFUNCTION()
	bool DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float SlopeRise, float SlopeRun, FVector2D origin);
	bool DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float XIntercept);

private:
	UFUNCTION()
	void DistrubuteHeat();

	//---Save Ship---
public:
	UFUNCTION(BlueprintCallable)
	void BuildShip(TArray<FSavePartInfo> Parts);

	UFUNCTION(BlueprintCallable)
	void SaveShip(FString ShipName);

	UFUNCTION(BlueprintCallable)
	bool LoadSavedShip(FString ShipName);



	//---Getters---

	UFUNCTION(BlueprintPure)
	const FVector2D GetCenterOfMass();

	UFUNCTION(BlueprintPure)
	const float GetMomentOfInertia();

	UFUNCTION(BlueprintPure)
	const float GetMass();

	UFUNCTION(BlueprintPure)
	TMap<FIntPoint, FPartData> GetPartGrid();

private:
	UPROPERTY()
	TMap<FIntPoint, FPartData> PartGrid;

	UPROPERTY()
	FArrayBounds GridBounds;

	UPROPERTY(EditAnywhere)
	FIntPoint GridSize;

	UPROPERTY(EditAnywhere)
	float GridScale; 

	UPROPERTY(EditAnywhere)
	class UStaticMesh* PixelMesh;

	UPROPERTY(EditAnywhere)
	float HeatTickRate;
	UPROPERTY(EditAnywhere, meta=(ClampMin="0", ClampMax="1"))
	float HeatPropagationFactor;

	UPROPERTY()
	float TimesSinceHeatTick;

	UFUNCTION()
	bool const CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape);


};
