// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "VoidsingerTypes.generated.h"

class UBasePart;

UENUM(BlueprintType)
enum EDifficulty
{
	Easy			UMETA(DisplayName = "Easy"),
	Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	VeryHard		UMETA(DisplayName = "Very Hard"),
};

UENUM(BlueprintType)
enum EResourceType
{
	Fuel			UMETA(DisplayName = "Fuel"),
	Hydrogen		UMETA(DisplayName = "Hydrogen"),
	InertGas		UMETA(DisplayName = "InertGas"),
	Coolent			UMETA(DisplayName = "Coolent"),
	Electricity		UMETA(DisplayName = "Electricity"),
	Ammo			UMETA(DisplayName = "Ammo"),
};

UENUM(BlueprintType)
enum EVoidsongVerbs
{
	Activate		UMETA(DisplayName = "Activate"),
	Deactivate		UMETA(DisplayName = "Deactivate"),
	GitGud			UMETA(DisplayName = "GitGud"),
	BecomNube		UMETA(DisplayName = "BecomNube"),
	Hot				UMETA(DisplayName = "Hot"),
	Cold			UMETA(DisplayName = "Cold"),
	Absorb			UMETA(DisplayName = "Absorb"),
	Disperse		UMETA(DisplayName = "Disperse"),

};

//Faction types. Enemy will probably be changed but it works for now.
UENUM(BlueprintType)
enum EFactions
{
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
	Inanimate		UMETA(DisplayName = "Inanimate")
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FArrayBounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint LowerBounds = FIntPoint(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint UpperBounds = FIntPoint(0);

	FArrayBounds()
	{
		LowerBounds = FIntPoint(0, 0);
		UpperBounds = FIntPoint(0, 0);
	}

	FArrayBounds(FIntPoint Lower, FIntPoint Upper)
	{
		LowerBounds = Lower;
		UpperBounds = Upper;
	}

	FArrayBounds(int32 LowerX, int32 LowerY, int32 UpperX, int32 UpperY)
	{
		LowerBounds = FIntPoint(LowerX, LowerY);
		UpperBounds = FIntPoint(UpperX, UpperY);
	}
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FIntPointArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntPoint> IntPointArray;

	FIntPointArray()
	{
		FIntPointArray(TArray<FIntPoint>(0,0));
	}


	FIntPointArray(TArray<FIntPoint> Array)
	{
		IntPointArray = Array;
	}
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FUBasePartArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UBasePart*> UBasePartArray;

	FUBasePartArray()
	{
		FUBasePartArray(TArray<UBasePart*>(0,0));
	}


	FUBasePartArray(TArray<UBasePart*> Array)
	{
		UBasePartArray = Array;
	}
};

USTRUCT(BlueprintType)
struct VOIDSINGER_API FSavePartInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasePart> PartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PartRotation;

	FSavePartInfo()
	{
		FSavePartInfo(nullptr, FIntPoint(), 0);
	}

	FSavePartInfo(TSubclassOf<UBasePart> Class, FIntPoint Location, float Rotation)
	{

		PartClass = Class;
		PartLocation = Location;
		PartRotation = Rotation;
	}
};

USTRUCT(BlueprintType)
struct FTextureDataTable : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

};

template <class T>
class VOIDSINGER_API TGridMap
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TSet<FIntPoint> ValidLocations;

	UPROPERTY()
	TArray<T> Values;

	UPROPERTY()
	int32 GridHalfSize = 250;
public:
	FPartGrid()
	{
		FPartGrid(TSet<FIntPoint>(), TArray<T>());
	}

	FPartGrid(TSet<FIntPoint> Locations, TArray<T> MapValues, int GridHalfSize = 250)
	{
		Locations.SetNum(MapValues.Num());
		ValidLocations = Locations;
		Values = MapValues;
		GridHalfSize = GridHalfSize;
	}

	T Emplace(FIntPoint Location, T Value)
	{
		if (ValidLocations.Num() == 0)
		{
			ValidLocations.Emplace(Location);
			return Values[Values.Emplace(Value)];
		}


		int32 InsertionIndex = BinaryInsertionSearch(LocationToRelativeValue(Location));

		ValidLocations.EmplaceAt(InsertionIndex, Location);
		Values.EmplaceAt(InsertionIndex, Value);

		return Values[InsertionIndex];
	}

	void Remove(FIntPoint Location)
	{
		int32 RemovalIndex = BinarySearch(LocationToRelativeValue(Location));
		Remove(RemovalIndex);
	}
	void Remove(int32 Index)
	{
		if (ValidLocations.Contains(Index))
		{
			ValidLocations.RemoveAt(Index);
			Values.RemoveAt(Index);
		}
	}

	bool Contains(FIntPoint Location)
	{
		return BinarySearch(LocationToRelativeValue(Location)) >= 0;
	}

	T FindRef(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return Values[Index];
		}

		return T();
	}

	T* Find(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return &Values[Index];
		}
		return nullptr;
	}

	int32 Num()
	{
		return ValidLocations.Num();
	}

	TArray<T> GetValueArray()
	{
		return Values;
	}

	TArray<FIntPoint> GetKeyArray()
	{
		return ValidLocations;
	}

	FIntPoint& LocationAtIndex(int32 Index)
	{
		return ValidLocations[Index];
	}

	T& ValueAtIndex(int32 Index)
	{
		return Values[Index];
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