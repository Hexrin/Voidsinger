// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//The type of location data for TGridMap
typedef FIntPoint GridLocationType;


//Stores a Value at a Location
template <class ValueType>
struct FGridPair
{
public:
	//The Location of the Value
	GridLocationType Location;
	//The Value being stored
	ValueType Value;

	//Constructs a GridPair with a given Value at the given Location
	FGridPair(GridLocationType Loc = GridLocationType(), ValueType Val = ValueType())
	{
		Location = Loc;
		Value = Val;
	}
};


/*
Stores a grid with fixed dimensions of Values
Each Value is bound to a 2D integer coordinate Location
Has functions for determining adjacency and connectivity of values
*/
template <class ValueType>
class TGridMap
{
	/* typedefs *\
	\* \/    \/ */

private:
	//The type used for storing a Values and its Location
	typedef FGridPair<ValueType> ElementType;

	//The type used for storing Values and thier Locations
	typedef TArray<ElementType> GridInfoType;

	/* /\    /\ *\
	\* typedefs */
	/* Variables *\
	\* \/     \/ */

private:
	//Stores all values and thier locations
	GridInfoType GridInfo;

	//Stores the size of the grid
	GridLocationType GridSize;

	/* /\     /\ *\
	\* Variables */
	/* Constructors *\
	\* \/        \/ */

public:
	//Constructs a GridMap with the given dimensions and no Values
	TGridMap(GridLocationType GridDimensions = GridLocationType(500))
	{
		GridInfo = GridInfoType();
		GridSize = GridDimensions;
	}

	//Constructs a GridMap with the given dimensions and Values initilized using the default constuctor at all Locations
	TGridMap(TSet<GridLocationType> Locations, GridLocationType GridDimensions = GridLocationType(500))
	{
		TGridMap(Locations, ValueType(), GridDimensions);
	}

	//Constructs a GridMap with the given dimensions and creates copies of Value at all Locations
	TGridMap(TSet<GridLocationType> ValueLocations, ValueType Value, GridLocationType GridDimensions = GridLocationType(500))
	{
		GridSize = GridDimensions;

		for (GridLocationType LocationToAdd : ValueLocations)
		{
			Emplace(LocationToAdd, Value);
		}
	}

	//Constucts a GridMap with the given Values and dimensions
	TGridMap(GridInfoType Values, GridLocationType GridDimensions = GridLocationType(250))
	{
		GridInfo = Values.Sort(ComparePair);
		GridSize = GridDimensions;
	}

	/* /\        /\ *\
	\* Constructors */
	

	ValueType Emplace(GridLocationType Location, ValueType Value)
	{
		if (GridInfo.Num() == 0)
		{
			return GridInfo.Emplace_GetRef(ElementType(Location, Value)).Value;
		}


		int32 InsertionIndex = BinaryInsertionSearch(LocationToRelativeValue(Location));

		GridInfo.EmplaceAt(InsertionIndex, ElementType(Location, Value));

		return GridInfo[InsertionIndex].Value;
	}

	void Remove(GridLocationType Location)
	{
		int32 RemovalIndex = BinarySearch(LocationToRelativeValue(Location));
		Remove(RemovalIndex);
	}
	void Remove(int32 Index)
	{
		if (GridInfo.IsValidIndex(Index))
		{
			GridInfo.RemoveAt(Index);
		}
	}

	bool Contains(GridLocationType Location)
	{
		return BinarySearch(LocationToRelativeValue(Location)) >= 0;
	}

	ValueType FindRef(GridLocationType Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return ValueAtIndex(Index);
		}

		return ValueType();
	}

	ValueType* Find(GridLocationType Location)
	{
		int32 Index = BinarySearch(LocationToRelativeValue(Location));
		if (Index >= 0)
		{
			return &ValueAtIndex(Index);
		}
		return nullptr;
	}

	int32 Num()
	{
		return GridInfo.Num();
	}

	TArray<ValueType> GenerateValueArray()
	{
		TArray<ValueType> Values = TArray<ValueType>();

		for (ElementType GridInfoPair : GridInfo)
		{
			Values.Emplace(GridInfoPair.Value);
		}

		return Values;
	}

	TArray<GridLocationType> GenerateLocationArray()
	{
		TArray<GridLocationType> Loactions = TArray<GridLocationType>();

		for (ElementType GridInfoPair : GridInfo)
		{
			Loactions.Emplace(GridInfoPair.Location);
		}

		return Loactions;
	}

	GridLocationType& LocationAtIndex(int32 Index)
	{
		return GridInfo[Index].Location;
	}

	ValueType& ValueAtIndex(int32 Index)
	{
		return GridInfo[Index].Value;
	}

	bool PointsConnected(GridLocationType StartPoint, GridLocationType EndPoint)
	{
		return PointsConnected(StartPoint, EndPoint, AlwaysConnect);
	}
	bool PointsConnected(GridLocationType StartPoint, GridLocationType EndPoint, TArray<GridLocationType>& ConnectivityArray)
	{
		return PointsConnected(StartPoint, EndPoint, ConnectivityArray, AlwaysConnect);
	}
	template<typename BinaryPredicate>
	bool PointsConnected(GridLocationType StartPoint, GridLocationType EndPoint, BinaryPredicate ConectivityCondition)
	{
		//Initate Conectiveity Array
		TArray<GridLocationType> ConectivityArray = TArray<GridLocationType>();
		return PointsConnected(StartPoint, EndPoint, ConectivityArray, ConectivityCondition);
	}
	template<typename BinaryPredicate>
	bool PointsConnected(GridLocationType StartPoint, GridLocationType EndPoint, TArray<GridLocationType>& ConnectivityArray, BinaryPredicate ConectivityCondition)
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
			GridLocationType TargetPoint = StartPoint + ((!IsXCloser ^ (i % 2 == 1)) ? GridLocationType((XIsPosive ^ (i > 1)) ? 1 : -1, 0) : GridLocationType(0, (YIsPosive ^ (i > 1)) ? 1 : -1));
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

	static bool AlwaysConnect(ValueType Value, GridLocationType IntPoint)
	{
		return true;
	}

private:
	const static bool ComparePair(ElementType& A, ElementType& B)
	{
		return LocationToRelativeValue(A[0]) < LocationToRelativeValue(B[0]);
	}

	const int32 BinarySearch(int32 TargetValue)
	{
		if (GridInfo.Num() == 0)
		{
			return -1;
		}
		return BinarySearch(TargetValue, 0, GridInfo.Num() - 1);
	}

	const int32 BinarySearch(int32 TargetValue, int32 MinIndex, int32 MaxIndex)
	{
		if (MinIndex > MaxIndex)
		{
			return -1;
		}

		int32 IndexToCheck = (MaxIndex + MinIndex) / 2;
		int32 CheckValue = LocationToRelativeValue(GridInfo[IndexToCheck].Location);
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
		if (GridInfo.Num() == 0)
		{
			return 0;
		}
		return BinaryInsertionSearch(TargetValue, 0, GridInfo.Num() - 1);
	}

	const int32 BinaryInsertionSearch(int32 TargetValue, int32 MinIndex, int32 MaxIndex)
	{
		if (MinIndex > MaxIndex)
		{
			return FMath::Max(MinIndex, 0);
		}

		int32 IndexToCheck = (MaxIndex + MinIndex) / 2;
		int32 CheckValue = LocationToRelativeValue(GridInfo[IndexToCheck].Location);
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

	const int32 LocationToRelativeValue(GridLocationType Location)
	{
		Location.X = Location.X > 0 ? 2 * Location.X : -2 * Location.X - 1;
		Location.Y = Location.Y > 0 ? 2 * Location.Y : -2 * Location.Y - 1;
		return Location.X + GridSize.X * Location.Y;
	}

	const GridLocationType RelativeValueToLocation(int32 Index)
	{
		GridLocationType ReturnValue = GridLocationType(Index % GridSize.X, Index / (GridSize.Y / 2));
		ReturnValue.X = ReturnValue.X % 2 == 0 ? ReturnValue.X / 2 : (ReturnValue.X + 1) / -2;
		ReturnValue.Y = ReturnValue.Y % 2 == 0 ? ReturnValue.Y / 2 : (ReturnValue.Y + 1) / -2;
		return ReturnValue;
	}
};
