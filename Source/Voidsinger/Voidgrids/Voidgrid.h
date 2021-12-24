// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePartTypes.h"
#include "GridMap.h"
#include "Voidgrid.generated.h"

class UBasePart;

//Stores data about a pixel.
USTRUCT()
struct VOIDSINGER_API FGridPixelData
{
	GENERATED_BODY()
	
	//Stores a pointer to the part in this pixel.
	UBasePart* Part;
};

//Used for disbatching events requireing a grid locaiton
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGridLocationDelegate, FIntPoint, GridLocaction);

UCLASS()
class VOIDSINGER_API AVoidgrid : public APawn
{
	GENERATED_BODY()

public:
	//Sets default values for this voidgrid's properties
	AVoidgrid();

	/* ---------------- *\ 
	\* \/ Pixel Mold \/ */
public:
	//The type used for storing pixel data
	typedef FGridPixelData PixelType;
	//The type used for storing a ships Pixel Mold
	typedef TGridMap<PixelType> PixelMoldType;
	//Stores all information required to replicate a mold
	typedef TSet<FMinimalPartData> MinimalMoldDataType;

	FGridLocationDelegate OnDamaged;

	/**
	 * Sets the pixel mold of the ship
	 * 
	 * @param NewPixelMold - The value to assign to the pixel mold of the ship
	 */
	void SetPixelMold(MinimalMoldData NewPixelMold);

	/**
	 * Gets the part data for all parts on this void grid.
	 * 
	 * @return The part data for all parts on this void grid.
	 */
	MinimalMoldDataType GetMoldData();

private:
	//Stores the Pixel Mold of this ship.
	PixelMoldType PixelMold;
};
