// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidgrid.h"
#include "BasePart.h"

//Sets default values for this voidgrid's properties
AVoidgrid::AVoidgrid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/**
 * Sets the pixel mold of the ship
 *
 * @param NewPixelMold - The value to assign to the pixel mold of the ship
 */
void AVoidgrid::SetPixelMold(MinimalMoldDataType NewPixelMold)
{
	MinimalMoldDataType MoldData = GetMoldData();
	for (FMinimalPartData PartData : NewPixelMold)
	{
		if()
	}
	
}

/**
 * Gets the part data for all parts on this void grid.
 *
 * @return The part data for all parts on this void grid.
 */
MinimalMoldDataType AVoidgrid::GetMoldData()
{
	MinimalMoldDataType AllPartsData = MinimalMoldDataType();
	for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		AllPartsData.Add(PixelData.Value.Part->GetData());
	}
	return AllPartsData;
}
