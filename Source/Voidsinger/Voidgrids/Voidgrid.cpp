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
	MinimalMoldDataType DataOfPartsToCreate = NewPixelMold;
	//Remove Unneccesary Parts
	for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		FMinimalPartData PartData = FMinimalPartData(PixelData.Value.Part->StaticClass(), PixelData.Value.Part->GetTransform());
		if (!NewPixelMold.Contains(PartData))
		{
			PixelMold.Remove(PixelData.Key);
		}
		else
		{
			DataOfPartsToCreate.Remove(PartData);
		}
	}

	for (FMinimalPartData DataOfPartToCreate : DataOfPartsToCreate)
	{
		UBasePart* Part = UBasePart::CreatePart(this, FPartData(DataOfPartToCreate));
		Parts.Emplace(Part);

		for (GridLocationType ShapeComponent : Cast<UBasePart>(Part->StaticClass())->GetDefaultShape())
		{
			PixelMold.Emplace(Part->GetTransform().TransformGridLocation(ShapeComponent), PixelType(Part));
		}
	}
}

/**
 * Gets the minimal part data for all parts on this void grid.
 *
 * @return The minimal part data for all parts on this void grid.
 */
MinimalMoldDataType AVoidgrid::GetMinimalMoldData()
{
	MinimalMoldDataType AllPartsData = MinimalMoldDataType();
	for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		AllPartsData.Add(PixelData.Value.Part->GetMinimalData());
	}
	return AllPartsData;
}

/**
 * Gets the part data for all parts on this void grid.
 *
 * @return The part data for all parts on this void grid.
 */
MoldDataType AVoidgrid::GetMoldData()
{
	MoldDataType AllPartsData = MoldDataType();
	/*for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		AllPartsData.Add(PixelData.Value.Part->GetData());
	}*/
	return AllPartsData;
}