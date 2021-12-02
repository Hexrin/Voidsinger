// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "VoidsingerTypes.generated.h"

class UBasePart;

//Comment -Mabel Suggestion
UENUM(BlueprintType)
enum EDifficulty
{
	Easy			UMETA(DisplayName = "Easy"),
	Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	VeryHard		UMETA(DisplayName = "Very Hard"),
};

//Comment -Mabel Suggestion
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

//Faction types. Enemy will probably be changed but it works for now.
UENUM(BlueprintType)
enum EFactions
{
	Player			UMETA(DisplayName = "Player"),
	Enemy			UMETA(DisplayName = "Enemy"),
	Inanimate		UMETA(DisplayName = "Inanimate")
};

//Comment -Mabel Suggestion
//Does this need to be outside the part grid?
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

//Comment -Mabel Suggestion
USTRUCT(BlueprintType)
struct VOIDSINGER_API FSavePartInfo
{
	GENERATED_BODY()

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBasePart> PartClass;

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint PartLocation;

	//Comment -Mabel Suggestion
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PartRotation;

	//Comment -Mabel Suggestion
	FSavePartInfo()
	{
		FSavePartInfo(nullptr, FIntPoint(), 0);
	}

	//Comment -Mabel Suggestion
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


//Comment -Mabel Suggestion
template <class ValueType>
class TGridMap
{

public:

	//Comment -Mabel Suggestion
	TArray<FIntPoint> ValidLocations;

	//Comment -Mabel Suggestion
	TArray<ValueType> Values;

	//Comment -Mabel Suggestion
	//maGIC NUMBERS!!!!!!!!!!!!!!!!!!!!!!!!
	int32 GridHalfSize = 250;

public:

	//Comment -Mabel Suggestion
	TGridMap()
	{
		TGridMap(TSet<FIntPoint>(), TArray<ValueType>());
	}
	
	//Comment -Mabel Suggestion
	TGridMap(TSet<FIntPoint> Locations, ValueType MapValue, int NewGridHalfSize = 250)
	{
		TArray<ValueType> MapValues = TArray<ValueType>();
		MapValues.Init(MapValue, Locations.Num());
		TGridMap(Locations, MapValues, NewGridHalfSize);
		
	}

	//Comment -Mabel Suggestion
	TGridMap(TSet<FIntPoint> Locations, TArray<ValueType> MapValues, int NewGridHalfSize = 250)
	{
		MapValues.SetNum(Locations.Num());
		ValidLocations = Locations.Array();
		Values = MapValues;
		GridHalfSize = NewGridHalfSize;
	}

	//Comment -Mabel Suggestion
	//Why does emplace return a value?
	ValueType Emplace(FIntPoint Location, ValueType Value)
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

	//Comment -Mabel Suggestion
	void Remove(FIntPoint Location)
	{
		int32 RemovalIndex = BinarySearch(LocationToRelativeValue(Location));
		Remove(RemovalIndex);
	}

	//Comment -Mabel Suggestion
	void Remove(int32 Index)
	{
		if (ValidLocations.IsValidIndex(Index))
		{
			ValidLocations.RemoveAt(Index);
			Values.RemoveAt(Index);
		}
	}

	//Comment -Mabel Suggestion
	bool Contains(FIntPoint Location)
	{
		return BinarySearch(LocationToRelativeValue(Location)) >= 0;
	}

	//Comment -Mabel Suggestion
	ValueType FindRef(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return Values[Index];
		}

		return ValueType();
	}

	//Comment -Mabel Suggestion
	ValueType* Find(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return &Values[Index];
		}
		return nullptr;
	}

	//Comment -Mabel Suggestion
	int32 Num()
	{
		return ValidLocations.Num();
	}

	//Comment -Mabel Suggestion
	TArray<ValueType> GetValueArray()
	{
		return Values;
	}

	//Comment -Mabel Suggestion
	TArray<FIntPoint> GetKeyArray()
	{
		return ValidLocations;
	}

	//Comment -Mabel Suggestion
	FIntPoint& LocationAtIndex(int32 Index)
	{
		return ValidLocations[Index];
	}

	//Comment -Mabel Suggestion
	ValueType& ValueAtIndex(int32 Index)
	{
		return Values[Index];
	}

	//Comment -Mabel Suggestion
	template<typename BinaryPredicate>
	bool PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, BinaryPredicate ConectivityCondition)
	{
		//Initate Conectiveity Array
		TArray<FIntPoint> ConectivityArray = TArray<FIntPoint>();
		return PointsConnected(StartPoint, EndPoint, ConectivityArray, ConectivityCondition);
	}

	//Comment -Mabel Suggestion
	//Does template<typename BinaryPredicate> need to be here again after you put it above?
	template<typename BinaryPredicate>
	bool PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, TArray<FIntPoint>& ConnectivityArray, BinaryPredicate ConectivityCondition)
	{
		
		//Detect if funtion has reached target
		if (StartPoint == EndPoint)
		{
			return true;
		}

		//Prevent Infinte Loops
		ConnectivityArray.Emplace(StartPoint);

		//Initate Variables
		bool ReturnValue = false;
		const bool IsXCloser = abs((EndPoint - StartPoint).X) < abs((EndPoint - StartPoint).Y);
		bool XIsPosive = (EndPoint - StartPoint).X > 0;
		bool YIsPosive = (EndPoint - StartPoint).Y > 0;
		//UE_LOG(LogTemp, Warning, TEXT("Direction x=%i, y=%i"), (EndPoint - StartPoint).X, (EndPoint - StartPoint).Y);


		//Iterate though and run recursive function for all adjecent pixels
		for (int i = 0; i < 4; i++)
		{
			//Select next pixel to scan based of of direction to EndPoint
			FIntPoint TargetPoint = StartPoint + ((!IsXCloser ^ (i % 2 == 1)) ? FIntPoint((XIsPosive ^ (i > 1)) ? 1 : -1, 0) : FIntPoint(0, (YIsPosive ^ (i > 1)) ? 1 : -1));
			//UE_LOG(LogTemp, Warning, TEXT("Target Point x=%i, y=%i, Xclose=%i, Xpos=%i, Ypos=%i"), TargetPoint.X, TargetPoint.Y, (IsXCloser ^ (i % 2 == 1)) ? 1 : 0, !(XIsPosive ^ (i > 1)) ? 1 : 0, !(YIsPosive ^ (i > 1)) ? 1 : 0);
			
			//Scan Pixel
			if (!ConnectivityArray.Contains(TargetPoint) && Contains(TargetPoint) && ConectivityCondition(FindRef(TargetPoint), TargetPoint))
			{
				ReturnValue = PointsConnected(TargetPoint, EndPoint, ConnectivityArray, ConectivityCondition);
				if (ReturnValue)
				{
					break;
				}
			}
		}


		return ReturnValue;
	}



private:

	//Comment -Mabel Suggestion
	const int32 BinarySearch(int32 TargetValue)
	{
		if (ValidLocations.Num() == 0)
		{
			return -1;
		}
		return BinarySearch(TargetValue, 0, ValidLocations.Num() - 1);
	}

	//Comment -Mabel Suggestion
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

	//Comment -Mabel Suggestion
	const int32 BinaryInsertionSearch(int32 TargetValue)
	{
		if (ValidLocations.Num() == 0)
		{
			return 0;
		}
		return BinaryInsertionSearch(TargetValue, 0, ValidLocations.Num() - 1);
	}

	//Comment -Mabel Suggestion
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

	//Comment -Mabel Suggestion
	const int32 LocationToRelativeValue(FIntPoint Location)
	{
		Location.X = Location.X > 0 ? 2 * Location.X : -2 * Location.X - 1;
		Location.Y = Location.Y > 0 ? 2 * Location.Y : -2 * Location.Y - 1;
		return Location.X + 2 * GridHalfSize * Location.Y;
	}

	//Comment -Mabel Suggestion
	const FIntPoint RelativeValueToLocation(int32 Index)
	{
		FIntPoint ReturnValue = FIntPoint(Index % (2 * GridHalfSize), Index / GridHalfSize);
		ReturnValue.X = ReturnValue.X % 2 == 0 ? ReturnValue.X / 2 : (ReturnValue.X + 1) / -2;
		ReturnValue.Y = ReturnValue.Y % 2 == 0 ? ReturnValue.Y / 2 : (ReturnValue.Y + 1) / -2;
		return ReturnValue;
	}
};

//Comment -Mabel Suggestion
//Why does this take in parameters if it doesn't do anything with them?
template <class ValueType>
static bool AlwaysConnect(ValueType Value, FIntPoint IntPoint)
{
	return true;
}