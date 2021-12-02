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
#include "BasePart.h"
#include "PartGridComponent.generated.h"

class UBaseThrusterPart;
class UCorePart;

//Comment the struct, what's this used for? -Mabel
USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartData
{
	GENERATED_BODY()

	//Comment -Mabel
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBasePart* Part;

	//Comment -Mabel
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMat;

private:

	//Comment -Mabel
	UPROPERTY()
	int32 AdjacencyIndex = 0;

	//Comment -Mabel
	UPROPERTY()
	float Temperature = 0;

public:

	//Comment -Mabel
	FPartData()
	{
		FPartData(nullptr, 0, 0, nullptr);
	}

	//Comment -Mabel
	FPartData(UBasePart* PartRef, float Temp, int32 BitwiseIndex, UMaterialInterface* Material)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(Material, Cast<UObject>(PartRef));
		Part = PartRef;
		SetTemperature(Temp);
		SetAdjacencyIndex(BitwiseIndex);
	}

	//Comment -Mabel
	void SetTemperature(const float NewTemp)
	{
		Temperature = NewTemp;
		DynamicMat->SetScalarParameterValue("Temperature", Temperature);
	}

	//Comment -Mabel
	const float GetTemperature()
	{
		return Temperature;
	}

	//Comment -Mabel
	void SetAdjacencyIndex(int32 NewAdjacencyIndex)
	{
		AdjacencyIndex = NewAdjacencyIndex;
		DynamicMat->SetScalarParameterValue("Adjacency Index", AdjacencyIndex);
	}

	//Comment -Mabel
	const int32 GetAdjacencyIndex()
	{
		return AdjacencyIndex;
	}
};

//Comment the class maybe? -Mabel
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

	//Comment -Mabel
	UFUNCTION(BlueprintCallable)
	bool AddPart(TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace = false);
	bool AddPart(TArray<FIntPoint> PartialPartShape, TSubclassOf<UBasePart> PartType, FIntPoint Location, float Rotation, bool bAlwaysPlace = false);

	//Comment -Mabel
	UFUNCTION(BlueprintCallable)
	bool RemovePart(FIntPoint Location, bool CheckForBreaks = true);

	//---Destruction---

	//Destroys a pixel at a given location.
	//This might have too many parameters which is a sign of something spaghet but I added half of them so idk -Mabel
	UFUNCTION(BlueprintCallable)
	bool DestroyPixel(FIntPoint Location, class UBasePart*& DamagedPart, bool CheckForBreaks = true, bool FromExplosion = false, FVector ExplosionLocation = FVector(0, 0, 0), float ExplosionRadius = 0.0f);
	bool DestroyPixel(FIntPoint Location, bool CheckForBreaks = true, bool FromExplosion = false, FVector ExplosionLocation = FVector(0, 0, 0), float ExplosionRadius = 0.0f);

	//Removes the given shape from the part grid and create a new base ship with them.
	UFUNCTION()
	void RemoveDisconnectedShape(TArray<FIntPoint> Shape, bool FromExplosion, FVector ExplosionLocation, float ExplosionRadius);

	//Comment -Mabel
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

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	FPartData GetPartDataAtGridLocation(FIntPoint Location);

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	FPartData GetPartDataAtRelativeLocation(FVector Location);

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	FPartData GetPartDataAtWorldLocation(FVector Location);

private:

	//Comment -Mabel
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

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const FVector2D CalcCenterOfMass();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const float CalcMomentOfInertia();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const float CalcMass();

	//Comment -Mabel
	// Why is this not blueprint pure but GetPartGridScale is?
	//UFUNCTION(BlueprintPure)
	TGridMap<FPartData> GetPartGrid();
	
	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const float GetPartGridScale();

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartGridBounds();

	//Misc

	//Comment -Mabel
	UFUNCTION(BlueprintPure)
	bool PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, bool TestForFunctionality = false);

	//Comment -Mabel
	UFUNCTION()
	void UpdateMaterials(FIntPoint Location, TSubclassOf<UBasePart> PartType);

public:



private:

	//Comment -Mabel
	UPROPERTY()
	ABaseShip* Ship;

	//Comment -Mabel
	TGridMap<FPartData> PartGrid;

	//Comment -Mabel
	UPROPERTY()
	FArrayBounds GridBounds;

	//Comment -Mabel
	UPROPERTY(EditAnywhere)
	float GridScale; 

	//Comment -Mabel
	UPROPERTY(EditAnywhere)
	FIntPoint GridHalfSize;
	
	//The path to the pixel mesh. 
	UPROPERTY(EditAnywhere)
	FString PathToPixelMesh = TEXT("/Game/Parts/PlaneWithCollision.PlaneWithCollision");

	//Comment -Mabel
	UPROPERTY()
	UStaticMesh* PixelMesh;

	//Comment -Mabel
	UPROPERTY(EditAnywhere)
	float HeatTickRate;

	//Comment -Mabel
	UPROPERTY(EditAnywhere, meta=(ClampMin="0", ClampMax="1"))
	float HeatPropagationFactor;

	//Comment -Mabel
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1"))
	float HeatMeltTransferFactor;

	//Comment -Mabel
	UPROPERTY()
	float TimesSinceHeatTick;

	//Comment -Mabel
	UPROPERTY()
	UCorePart* CorePart;

	//Comment -Mabel
	UFUNCTION()
	bool const CanShapeFit(FIntPoint Loc, TArray<FIntPoint> DesiredShape);

	/*Static Functions*\
	\*----------------*/
public:

	//A recursive function that will check the shape it's provided with for any parts that are not connected to each other
	static TArray<FIntPoint> FindConnectedShape(TArray<FIntPoint> Shape, TGridMap<FPartData> ConnectedPartsMap, bool CheckFunctionality = false);

	//Comment -Mabel
	static bool IsPixelFunctional(FPartData PixelValue, FIntPoint Loc);
};
