// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GridMap.generated.h"

/*
* Stores a grid with fixed dimensions of Values
* Each Value is bound to a 2D integer coordinate Location
* Has functions for determining adjacency and connectivity of values
*/
template <class ValueType>
class TGridMap
{

public:
	//Stores all values and thier locations
	TArray<TPair<FIntPoint, ValueType>> GridInfo;

	//Stores the size of the grid
	FIntPoint GridSize;
public:
	/* Constructors *\
	\* \/        \/ */

	//Constructs a GridMap with the given dimensions and no Values
	TGridMap(FIntPoint GridDimensions = FIntPoint(500))
	{
		TSet<TPair<FIntPoint, ValueType>>(), GridDimensions);
	}

	//Constructs a GridMap with the given dimensions and Values initilized using the default constuctor at all Locations
	TGridMap(TSet<FIntPoint> Locations, FIntPoint GridDimensions = FIntPoint(500))
	{
		TGridMap(Locations, ValueType(), GridDimensions);
	}

	//Constructs a GridMap with the given dimensions and creates copies of Value at all Locations
	TGridMap(TSet<FIntPoint> ValueLocations, ValueType Value, FIntPoint GridDimensions = FIntPoint(500))
	{
		TMap<FIntPoint, ValueType> Values = TMap<FIntPoint, ValueType>();

		for (FIntPoint LocationToAdd : ValueLocations)
		{
			Values.Emplace(LocationToAdd, Value);
		}

		TGridMap(Values, GridDimensions);
	}

	TGridMap(TArray<TPair<FIntPoint, ValueType>>, FIntPoint GridDimensions = FIntPoint(250))
	{
		GridInfo = Values;
		GridSize = GridDimensions;
	}

	/* /\        /\ *\
	\* Constructors */
	

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
			Values.RemoveAt(Index);
		}
	}

	bool Contains(FIntPoint Location)
	{
		return BinarySearch(LocationToRelativeValue(Location)) >= 0;
	}

	ValueType FindRef(FIntPoint Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return Values[Index];
		}

		return ValueType();
	}

	ValueType* Find(FIntPoint Location)
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

	TArray<ValueType> GetValueArray()
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

	ValueType& ValueAtIndex(int32 Index)
	{
		return Values[Index];
	}

	template<typename BinaryPredicate>
	bool PointsConnected(FIntPoint StartPoint, FIntPoint EndPoint, BinaryPredicate ConectivityCondition)
	{
		//Initate Conectiveity Array
		TArray<FIntPoint> ConectivityArray = TArray<FIntPoint>();
		return PointsConnected(StartPoint, EndPoint, ConectivityArray, ConectivityCondition);
	}

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

	static bool AlwaysConnect(ValueType Value, FIntPoint IntPoint)
	{
		return true;
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
