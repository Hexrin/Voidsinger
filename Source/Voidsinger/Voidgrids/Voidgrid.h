// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridMap.h"
#include "Voidgrid.generated.h"

class UBasePart;
struct FPartData;
struct FMinimalPartData;

//The type used for storing pixel data
typedef FGridPixelData PixelType;
//The type used for storing a ships Pixel Mold
typedef TGridMap<PixelType> PixelMoldType;
//Stores all information required to replicate a mold
typedef TSet<FMinimalPartData> MinimalMoldDataType;
//Stores all information required to replicate a mold
typedef TSet<FPartData> MoldDataType;

//Stores data about a pixel.
USTRUCT()
struct VOIDSINGER_API FGridPixelData
{
	GENERATED_BODY()
	
public:
	//Stores a pointer to the part in this pixel.
	UBasePart* Part;

	//Constructs a FGridPixelData using a part.
	FGridPixelData(UBasePart* PartAtPixel = nullptr)
	{
		SetPart(PartAtPixel);
	}

	/**
	 * Sets the part of this pixel
	 * 
	 * @param NewPart - The new value of part.
	 * @return A refernce to this.
	 */
	FGridPixelData SetPart(UBasePart* NewPart)
	{
		Part = NewPart;
		return *this;
	}
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
	//Called when this is damaged
	FGridLocationDelegate OnDamaged;
	//Called when this is damaged
	FGridLocationDelegate OnRepaired;

	/**
	 * Sets the pixel mold of the ship
	 * 
	 * @param NewPixelMold - The value to assign to the pixel mold of the ship
	 */
	void SetPixelMold(MinimalMoldDataType NewPixelMold);

	/**
	 * Gets the minimal part data for all parts on this void grid.
	 * 
	 * @return The minimal part data for all parts on this void grid.
	 */
	MinimalMoldDataType GetMinimalMoldData();

	/**
	 * Gets the part data for all parts on this void grid.
	 *
	 * @return The part data for all parts on this void grid.
	 */
	MoldDataType GetMoldData();

private:
	//Stores the Pixel Mold of this.
	PixelMoldType PixelMold;

	//Stores a refernce to all parts on this.
	UPROPERTY()
	TSet<UBasePart*> Parts;
};
