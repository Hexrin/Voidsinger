// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidgrid.h"

//Sets default values for this voidgrid's properties
AVoidgrid::AVoidgrid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/**
* Adds a part to this ships Pixel Mold.
*
* @param PartClass - The class of part to add.
* @param Location - The location to add the part at.
* @return Whether or not a part was placed successfuly.
*/
bool AVoidgrid::AddPart(TSubclassOf<UBasePart> PartClass, GridLocationType Location)
{
	for (GridLocationType PixelPartRelativeLocation : PartClass.GetDefaultObject()->GetDesiredShape())
	{

	}
	return false;
}
