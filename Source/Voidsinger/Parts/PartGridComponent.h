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



USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBasePart* Part;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMat;

private:
	UPROPERTY()
	int32 AdjacencyIndex = 0;

	UPROPERTY()
	float Temperature = 0;

public:
	FPartData()
	{
		FPartData(nullptr, 0, 0, nullptr);
	}

	FPartData(UBasePart* PartRef, float Temp, int32 BitwiseIndex, UMaterialInterface* Material)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(Material, Cast<UObject>(PartRef));
		Part = PartRef;
		SetTemperature(Temp);
		SetAdjacencyIndex(BitwiseIndex);
	}

	void SetTemperature(const float NewTemp)
	{
		Temperature = NewTemp;
		DynamicMat->SetScalarParameterValue("Temperature", Temperature);
	}

	const float GetTemperature()
	{
		return Temperature;
	}

	void SetAdjacencyIndex(int32 NewAdjacencyIndex)
	{
		AdjacencyIndex = NewAdjacencyIndex;
		DynamicMat->SetScalarParameterValue("Adjacency Index", AdjacencyIndex);
	}

	const int32 GetAdjacencyIndex()
	{
		return AdjacencyIndex;
	}
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartGrid
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<FIntPoint> ValidLocations;

	UPROPERTY()
	TArray<FPartData> Parts;

	UPROPERTY()
	int32 GridHalfSize = 250;
public:
	FPartGrid()
	{
		ValidLocations = TArray<FIntPoint>();
		Parts = TArray<FPartData>();
	}

	FPartData Emplace(FIntPoint Location, FPartData PartData)
	{
		if (ValidLocations.Num() == 0)
		{
			ValidLocations.Emplace(Location);
			return Parts[Parts.Emplace(PartData)];
		}


		int32 InsertionIndex = BinaryInsertionSearch(LocationToRelativeValue(Location));
		
		ValidLocations.EmplaceAt(InsertionIndex, Location);
		Parts.EmplaceAt(InsertionIndex, PartData);

		return Parts[InsertionIndex];
	}
	
	void Remove(FIntPoint Location)
	{
		int32 RemovalIndex = BinarySearch(LocationToRelativeValue(Location));
		Remove(RemovalIndex);
	}
	void Remove(int32 Index)
	{
		if (ValidLocations.IsValidIndex(Index))
		{
			ValidLocations.RemoveAt(Index);
			Parts.RemoveAt(Index);
		}
	}

	bool Contains(FIntPoint Location)
	{
		return BinarySearch(LocationToRelativeValue(Location)) >= 0;
	}

	FPartData FindRef(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return Parts[Index];
		}
		
		return FPartData();
	}

	FPartData* Find(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return &Parts[Index];
		}
		return nullptr;
	}

	int32 Num()
	{
		return ValidLocations.Num();
	}

	TArray<FPartData> GetValueArray()
	{
		return Parts;
	}

	TArray<FIntPoint> GetKeyArray()
	{
		return ValidLocations;
	}

	FIntPoint& LocationAtIndex (int32 Index)
	{
		return ValidLocations[Index];
	}
	
	FPartData& PartDataAtIndex (int32 Index)
	{
		return Parts[Index];
	}
private:
	const int32 BinarySearch(int32 TargetValue)
	{
		if (ValidLocations.Num() == 0)
		{
			return -1;
		}
		return BinarySearch(TargetValue, 0, ValidLocations.Num() - 1);
	}
	
	const int32 BinarySearch(int32 TargetValue, int32 MinIndex, int32 MaxIndex)
	{
		if (MinIndex > MaxIndex)
		{
			return -1;
		}
		
		int32 IndexToCheck = (MaxIndex + MinIndex) / 2;
		int32 CheckValue = LocationToRelativeValue(ValidLocations[IndexToCheck]);
		if (CheckValue == TargetValue)
		{
			return IndexToCheck;
		}
		else if (CheckValue < TargetValue)
		{
			return BinarySearch(TargetValue, IndexToCheck + 1, MaxIndex);
		}
		else
		{
			return BinarySearch(TargetValue, MinIndex, IndexToCheck - 1);
		}
	}
	const int32 BinaryInsertionSearch(int32 TargetValue)
	{
		if (ValidLocations.Num() == 0)
		{
			return 0;
		}
		return BinaryInsertionSearch(TargetValue, 0, ValidLocations.Num() - 1);
	}

	const int32 BinaryInsertionSearch(int32 TargetValue, int32 MinIndex, int32 MaxIndex)
	{
		if (MinIndex > MaxIndex)
		{
			return FMath::Max(MinIndex, 0);
		}

		int32 IndexToCheck = (MaxIndex + MinIndex) / 2;
		int32 CheckValue = LocationToRelativeValue(ValidLocations[IndexToCheck]);
		if (CheckValue == TargetValue)
		{
			return IndexToCheck;
		}
		else if (CheckValue < TargetValue)
		{
			return BinaryInsertionSearch(TargetValue, IndexToCheck + 1, MaxIndex);
		}
		else
		{
			return BinaryInsertionSearch(TargetValue, MinIndex, IndexToCheck - 1);
		}
	}

	const int32 LocationToRelativeValue(FIntPoint Location)
	{
		Location.X = Location.X > 0 ? 2 * Location.X : -2 * Location.X - 1;
		Location.Y = Location.Y > 0 ? 2 * Location.Y : -2 * Location.Y - 1;
		return Location.X + 2 * GridHalfSize * Location.Y;
	}

	const FIntPoint RelativeValueToLocation(int32 Index)
	{
		FIntPoint ReturnValue = FIntPoint(Index % (2 * GridHalfSize), Index / GridHalfSize);
		ReturnValue.X = ReturnValue.X % 2 == 0 ? ReturnValue.X / 2 : (ReturnValue.X + 1) / -2;
		ReturnValue.Y = ReturnValue.Y % 2 == 0 ? ReturnValue.Y / 2 : (ReturnValue.Y + 1) / -2;
		return ReturnValue;
	}
};


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

	UFUNCTION(BlueprintPure)
	FPartData GetPartDataAtGridLocation(FIntPoint Location);

	UFUNCTION(BlueprintPure)
	FPartData GetPartDataAtRelativeLocation(FVector Location);

	UFUNCTION(BlueprintPure)
	FPartData GetPartDataAtWorldLocation(FVector Location);
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
	const FVector2D CalcCenterOfMass();

	UFUNCTION(BlueprintPure)
	const float CalcMomentOfInertia();

	UFUNCTION(BlueprintPure)
	const float CalcMass();

	UFUNCTION(BlueprintPure)
	FPartGrid GetPartGrid();

	UFUNCTION(BlueprintPure)
	const float GetPartGridScale();

	UFUNCTION(BlueprintPure)
	const FArrayBounds GetPartGridBounds();

	//Misc

	UFUNCTION()
	void UpdateMaterials(FIntPoint Location, TSubclassOf<UBasePart> PartType);

public:
	


private:
	UPROPERTY()
	ABaseShip* Ship;

	UPROPERTY()
	FPartGrid PartGrid;

	UPROPERTY()
	FArrayBounds GridBounds;

	UPROPERTY(EditAnywhere)
	float GridScale; 

	UPROPERTY(EditAnywhere)
	FIntPoint GridHalfSize;
	
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

	/*Static Functions*\
	\*----------------*/
public:
	UFUNCTION(BlueprintPure)
	static bool PointsConnected(FPartGrid Grid, FIntPoint StartPoint, FIntPoint EndPoint, bool TestForFunctionality = false);
	static bool PointsConnected(FPartGrid Grid, FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray, bool TestForFunctionality = false);

	//A recursive function that will check the shape it's provided with for any parts that are not connected to each other
	UFUNCTION()
	static TArray<FIntPoint> FindConnectedShape(TArray<FIntPoint> Shape, FPartGrid ConnectedPartsMap, bool CheckFunctionality = false);

};
