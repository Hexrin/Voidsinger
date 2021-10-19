// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Voidsinger/SaveShip.h"
#include "DrawDebugHelpers.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/Char.h"
#include "PartGridComponent.generated.h"

class UBasePart;
class UBaseThrusterPart;
class UCorePart;

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
	bool RemovePart(FIntPoint Location, bool CheckForBreaks = true);

	//---Destruction---

	//Destroys a pixel at a given location.
	UFUNCTION(BlueprintCallable)
	bool DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart, bool CheckForBreaks = true, bool FromExplosion = false, FVector ExplosionLocation = FVector(0, 0, 0), float ExplosionRadius = 0.0f);
	bool DestroyPixel(FIntPoint Location, bool CheckForBreaks = true, bool FromExplosion = false, FVector ExplosionLocation = FVector(0, 0, 0), float ExplosionRadius = 0.0f);

	//Removes the given shape from the part grid and create a new base ship with them.
	UFUNCTION()
	void RemoveDisconnectedShape(TArray<FIntPoint> Shape, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius);

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

	//Returns true if the given line intersects the given 2d rectangle
	UFUNCTION()
	bool DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float SlopeRise, float SlopeRun, FVector2D origin);
	bool DoesLineIntersectBox(FVector2D TopLeft, FVector2D BottomRight, float XIntercept);

private:
	UFUNCTION()
	void DistrubuteHeat();

	//---Save Ship---
public:

	//Builds a ship with the given parts
	UFUNCTION(BlueprintCallable)
	void BuildShip(TArray<FSavePartInfo> Parts);

	//Saves a ship to a save game object
	UFUNCTION(BlueprintCallable)
	void SaveShip(FString ShipName);

	//Loads a ship from a save game object
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

	UFUNCTION(BlueprintPure)
	const float GetPartGridScale();

	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartGridBounds();

	//Misc

	UFUNCTION()
	void UpdateMaterials(FIntPoint Location, TSubclassOf<UBasePart> PartType);

private:
	UPROPERTY()
	TMap<FIntPoint, FPartData> PartGrid;

	UPROPERTY()
	FArrayBounds GridBounds;

	UPROPERTY(EditAnywhere)
	FIntPoint GridSize;

	UPROPERTY(EditAnywhere)
	float GridScale; 

	//The path to the pixel mesh. 
	UPROPERTY(EditAnywhere)
	FString PathToPixelMesh = TEXT("/Game/Parts/PlaneWithCollision.PlaneWithCollision");

	UPROPERTY()
	UStaticMesh* PixelMesh;

	UPROPERTY(EditAnywhere)
	float HeatTickRate;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0", ClampMax="1"))
	float HeatPropagationFactor;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float HeatMeltTransferFactor;

	UPROPERTY()
	float TimesSinceHeatTick;

	UPROPERTY()
	UCorePart* CorePart;

	UFUNCTION()
	bool const CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape);


};
