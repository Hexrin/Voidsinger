// Fill out your copyright notice in the Description page of Project Settings.


#include "PartEditorBase.h"

/**
 * Gets the mutable default of the part this is curently editing.
 *
 * @return The mutable default of the part this is curently editing.
 */
UBasePart* UPartEditorBase::GetMutableDefaultPart()
{
	return GetMutableDefault<UBasePart>(PartBeingEdited);
}

/**
 * Gets the shape of the part this is curently editing.
 *
 * @return The locations of all the pixels of the shape.
 */
TArray<FIntPoint> UPartEditorBase::GetPartShape()
{
	return GetMutableDefaultPart()->DefaultShape.Array();
}

/**
 * Sets the shape of the part this is curently editing.
 *
 * @param NewShape - The locations of all the pixels of the shape.
 */
void UPartEditorBase::SetPartShape(TArray<FIntPoint> NewShape)
{
	GetMutableDefaultPart()->DefaultShape = TSet<FIntPoint>(NewShape);
	SavePart();
}

/**
 * Adds a pixel to the shape of the part this is curently editing.
 *
 * @param PixelLocation - The location of the pixel to add.
 */
void UPartEditorBase::AddPixelToPartShape(FIntPoint PixelLocation)
{
	GetMutableDefaultPart()->DefaultShape.Add(PixelLocation);
	SavePart();
}

/**
 * Removes a pixel from the shape of the part this is curently editing.
 *
 * @param PixelLocation - The location of the pixel to remove.
 */
void UPartEditorBase::RemovePixelFromPartShape(FIntPoint PixelLocation)
{
	GetMutableDefaultPart()->DefaultShape.Remove(PixelLocation);
	SavePart();
}

/**
 * Saves any changes made to the mutable default part.
 */
void UPartEditorBase::SavePart()
{
	GetMutableDefaultPart()->UpdateDefaultConfigFile();
}
