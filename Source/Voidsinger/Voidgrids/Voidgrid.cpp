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
	//Iterates though the pixel mold adding, removing and edditng pixel data to match the new pixel mold.
	for (TPair<GridLocationType, PixelType> PixelData : PixelMold.GetGridPairs())
	{
		UBasePart* PartRef = PixelData.Value.Part;
		FPartTransform PartTransform = PartRef->GetTransform();
		FMinimalPartData PartData = FMinimalPartData(PartRef->StaticClass(), PartTransform);

		TSet<GridLocationType> PartShapeComponentsToBeRemoved = PartRef->GetShape();
		//Edits the PixelMold to contain all desiered part shape component locations.
		if (NewPixelMold.Contains(PartData))
		{
			//                                     |                                Gets the desired shape of 
			for (GridLocationType ShapeComponent : Cast<UBasePart>(NewPixelMold.Find(PartData)->Class->GetDefaultObject())->GetDefaultShape())
			{
				GridLocationType EmplaceLoc = PartTransform.TransformGridLocation(ShapeComponent);
				PartShapeComponentsToBeRemoved.Remove(ShapeComponent);
				//Create new pixel data if Pixel data does not exist otherwise use old pixel data.
				PixelType NewPixelData = PixelMold.Contains(EmplaceLoc) ? PixelMold.Find(EmplaceLoc)->SetPart(PartRef) : PixelType(PartRef);
				PixelMold.Emplace(EmplaceLoc, NewPixelData);
			}
		}
		//Prevent part duplication and unnessecary logic.
		NewPixelMold.Remove(PartData);

		//Removes Unused Parts
		for (GridLocationType ShapeComponent : PartShapeComponentsToBeRemoved)
		{
			PixelMold.Remove(PartTransform.TransformGridLocation(ShapeComponent));
		}
		Parts.Remove(PartRef);		
	}
	
	//Creates parts not added to the 
	for(FMinimalPartData PartData : NewPixelMold)
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
