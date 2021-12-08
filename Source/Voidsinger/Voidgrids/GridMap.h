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
	/* -------------- *\
	\* \/ typedefs \/ */

private:
	//The type used for storing a Values and its Location
	typedef TPair<GridLocationType, ValueType> LocationValuePairType;

	//The type used for storing Values and thier Locations
	typedef TMap<GridLocationType, ValueType> GridInfoType;

	/* /\ typedefs /\ *\
	\* -------------- */

	/* --------------- *\
	\* \/ Variables \/ */

private:
	//Stores all values and thier locations
	GridInfoType GridInfo;

	//Stores the size of the grid
	GridLocationType GridSize;

	/* /\ Variables /\ *\
	\* --------------- */

	/* ------------------ *\
	\* \/ Constructors \/ */

public:
	//Constructs a GridMap with the given dimensions and no Values.
	TGridMap(GridLocationType GridDimensions = GridLocationType(500))
	{
		GridInfo = GridInfoType();
		GridSize = GridDimensions;
	}

	//Constructs a GridMap with the given dimensions and Values initilized using the default constuctor at all Locations.
	TGridMap(TSet<GridLocationType> Locations, GridLocationType GridDimensions = GridLocationType(500))
	{
		TGridMap(Locations, ValueType(), GridDimensions);
	}

	//Constructs a GridMap with the given dimensions and creates copies of Value at all Locations.
	TGridMap(TSet<GridLocationType> ValueLocations, ValueType Value, GridLocationType GridDimensions = GridLocationType(500))
	{
		GridSize = GridDimensions;

		for (GridLocationType LocationToAdd : ValueLocations)
		{
			Emplace(LocationToAdd, Value);
		}
	}

	//Constucts a GridMap with the given Values and dimensions.
	TGridMap(GridInfoType Values, GridLocationType GridDimensions = GridLocationType(250))
	{
		GridInfo = Values.Sort(LocationIDLessThan);
		GridSize = GridDimensions;
	}

	/* /\ Constructors /\ *\
	\* ------------------ */

	/* ---------------------- *\
	\* \/ Data Manipulation \/ */

public:
	/**
	 * Constructs a new item at a specified location.
	 *
	 * @param Location - The location to add the item at.
	 * @param Value	- The value of the item
	 * @return A reference to the newly-inserted item.
	 */
	ValueType Emplace(GridLocationType Location, ValueType Value = ValueType())
	{
		return GridInfo.Emplace(Location, Value);
	}

	/**
	 * Removes an item at a specified location.
	 *
	 * @param Location - The location to remove the item at.
	 */
	void Remove(GridLocationType Location)
	{
		GridInfo.Remove(Location);
	}

	/* /\ Data Manipulation /\ *\
	\* ----------------------- */

	/* ------------ *\
	\* \/ Search \/ */

public:
	/**
	 * Checks if there is a value at a given location.
	 *
	 * @param Location - The location to seach for.
	 * @return Whether or not the location has a value.
	 */
	bool Contains(GridLocationType Location)
	{
		return GridInfo.Contains(Location);
	}

	/**
	 * Finds the value at a given location.
	 *
	 * @param Location - The location to seach at.
	 * @return The value at Location.
	 */
	ValueType* Find(GridLocationType Location)
	{
		if (Contains(Location))
		{
			return &GridInfo[Location];
		}

		return nullptr;
	}

	/**
	 * Finds the value at a given location.
	 *
	 * @param Location - The location to seach at.
	 * @return A referce to the value at Location.
	 */
	ValueType FindRef(GridLocationType Location)
	{
		if (Contains(Location))
		{
			return GridInfo[Location];
		}

		return ValueType();
	}

	/* /\ Search /\ *\
	\* ------------ */

	/* ---------------- *\
	\* \/ Grid State \/ */

public:	
	/**
	 * Returns the number of values in a GridMap.
	 *
	 * @return The number of values in a GridMap.
	 */
	const int32 Num()
	{
		return GridInfo.Num();
	}

	/**
	 * Gets all Location Value Pairs.
	 *
	 * @return An array of all Location Value Pairs.
	 */
	GridInfoType GetGridPairs()
	{
		return GridInfo;
	}

	/**
	 * Creates an Array containing all Values in the GridMap.
	 * Use GetGridPairs() unless a return type of TArray<ValueType> is required.
	 *
	 * @return An Array containing all Values in the GridMap.
	 */
	TArray<ValueType> GenerateValueArray()
	{
		return GridInfo.GenerateValueArray();
	}

	/**
	 * Creates an Array containing all Locations with values in the GridMap.
	 * Use GetGridPairs() unless a return type of TArray<GridLocationType> is required.
	 *
	 * @return An Array containing all Locations with values in the GridMap.
	 */
	TArray<GridLocationType> GenerateLocationArray()
	{
		return GridInfo.GenerateKeyArray();
	}

	/* /\ Grid State /\ *\
	\* ---------------- */

	/* ------------------------- *\
	\* \/ Grid Shape Analysis \/ */

public:
	/**
	 * Gets the Location Value Pairs that satisfy the given predicate.
	 * 
	 * @param InclusionCondition - If InclusionCondition(LocationValuePairType) is true then the pair will be included in the return value.
	 * @return An array of Location Value Pairs that satisfy the given predicate.
	 */
	template<typename Predicate>
	TArray<LocationValuePairType> FilterGridPairs(Predicate InclusionCondition)
	{
		TArray<LocationValuePairType> Pairs = TArray<LocationValuePairType>();
		for (LocationValuePairType GridInfoPair : GridInfo)
		{
			if (InclusionCondition(GridInfoPair))
			{
				Pairs.Emplace(GridInfoPair.Location);
			}
		}

		return Pairs;
	}

	/**
	 * Checks if StartLocation is connected connected to EndLocation in the grid.
	 * 
	 * @param StartLocation - The location in gridspace to begin the connectivty search.
	 * @param EndLocation -  The target location in gridspace for the connectivty search.
	 * @return Whether or not StartLocation is connected to EndLocation.
	 */
	bool PointsConnected(GridLocationType StartLocation, GridLocationType EndLocation)
	{
		return PointsConnected(StartLocation, EndLocation, AlwaysConnect);
	}

	/**
	 * Checks if StartLocation is connected connected to EndLocation in the grid by Location Value Pairs that satisfy the given predicate.
	 *
	 * @param StartLocation - The location in gridspace to begin the connectivty search.
	 * @param EndLocation - The target location in gridspace for the connectivty search.
	 * @param ConectivityCondition - If ConectivityCondition(LocationValuePairType) is false than that location is not considered when searching for valid conections.
	 * @return Whether or not StartLocation is connected to EndLocation by Location Value Pairs that satisfy the given predicate.
	 */
	template<typename Predicate>
	bool PointsConnected(GridLocationType StartLocation, GridLocationType EndLocation, Predicate ConectivityCondition)
	{
		//Initate Conectiveity Array
		TArray<GridLocationType> ConectivityArray = TArray<GridLocationType>();
		return PointsConnected(StartLocation, EndLocation, ConectivityArray, ConectivityCondition);
	}

private:
	/**
	 * Checks if StartLocation is connected connected to EndLocation in the grid.
	 *
	 * @param StartLocation - The location in gridspace to begin the connectivty search.
	 * @param EndLocation -  The target location in gridspace for the connectivty search.
	 * @param SeachedLocations - An array storing locations already searched.
	 * @return Whether or not StartLocation is connected to EndLocation.
	 */
	bool PointsConnected(GridLocationType StartLocation, GridLocationType EndLocation, TArray<GridLocationType>& SeachedLocations)
	{
		return PointsConnected(StartLocation, EndLocation, SeachedLocations, AlwaysConnect);
	}

	/**
	 * Checks if StartLocation is connected connected to EndLocation in the grid.
	 *
	 * @param StartLocation - The location in gridspace to begin the connectivty search.
	 * @param EndLocation -  The target location in gridspace for the connectivty search.
	 * @param SeachedLocations - An array storing locations already searched.
	 * @param ConectivityCondition - If ConectivityCondition(LocationValuePairType) is false than that location is not considered when searching for valid conections.
	 * @return Whether or not StartLocation is connected to EndLocation.
	 */
	template<typename Predicate>
	bool PointsConnected(GridLocationType StartLocation, GridLocationType EndLocation, TArray<GridLocationType>& SeachedLocations, Predicate ConectivityCondition)
	{
		//Detect if funtion has reached target
		if (StartLocation == EndLocation)
		{
			return true;
		}

		//Prevent Infinte Loops
		SeachedLocations.Emplace(StartLocation);

		//Initate Variables
		bool ReturnValue = false;
		const bool IsXCloser = abs((EndLocation - StartLocation).X) < abs((EndLocation - StartLocation).Y);
		bool XIsPosive = (EndLocation - StartLocation).X > 0;
		bool YIsPosive = (EndLocation - StartLocation).Y > 0;

		//Iterate though and run recursive function for all adjecent pixels
		for (int i = 0; i < 4; i++)
		{
			//Select next pixel to scan based of of direction to EndLocation
			GridLocationType TargetLocation = StartLocation + ((!IsXCloser ^ (i % 2 == 1)) ? GridLocationType((XIsPosive ^ (i > 1)) ? 1 : -1, 0) : GridLocationType(0, (YIsPosive ^ (i > 1)) ? 1 : -1));
			
			//Check to see if TargetLocation contains a value and satisfies the Conectivity Condition.
			if (!SeachedLocations.Contains(TargetLocation) && Contains(TargetLocation) && ConectivityCondition(LocationValuePairType(TargetLocation, FindRef(TargetLocation))))
			{
				ReturnValue = PointsConnected(TargetLocation, EndLocation, SeachedLocations, ConectivityCondition);
				if (ReturnValue)
				{
					break;
				}
			}
		}


		return ReturnValue;
	}

	/**
	 * Helper Function for PointsConnected. Allways returns true.
	 * 
	 * @return true
	 */
	static bool AlwaysConnect(LocationValuePairType Value)
	{
		return true;
	}

	/* /\ Grid Shape Analysis /\ *\
	\* ------------------------- */
};
