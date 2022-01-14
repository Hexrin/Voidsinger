// Fill out your copyright notice in the Description page of Project Settings.


#include "PartEditorBase.h"

/**
 * Gets the mutable default of the part this is curently editing.
 *
 * @return The mutable default of the part this is curently editing.
 */
UPart* UPartEditorBase::GetMutableDefaultPart()
{
	if (IsValid(PartBeingEdited))
	{
		return GetMutableDefault<UPart>(PartBeingEdited);
	}
	return nullptr;
}

/**
 * Gets the shape of the part this is curently editing.
 *
 * @return The locations of all the pixels of the shape.
 */
TArray<FIntPoint> UPartEditorBase::GetPartShape()
{
	if (IsValid(GetMutableDefaultPart()))
	{
		return GetMutableDefaultPart()->DefaultShape.Array();
	}
	return TArray<FIntPoint>();
}

/**
 * Sets the shape of the part this is curently editing.
 *
 * @param NewShape - The locations of all the pixels of the shape.
 */
void UPartEditorBase::SetPartShape(TArray<FIntPoint> NewShape)
{
	if (IsValid(GetMutableDefaultPart()))
	{
		GetMutableDefaultPart()->DefaultShape = TSet<FIntPoint>(NewShape);
	}
}

/**
 * Adds a pixel to the shape of the part this is curently editing.
 *
 * @param PixelLocation - The location of the pixel to add.
 */
void UPartEditorBase::AddPixelToPartShape(FIntPoint PixelLocation)
{
	if (IsValid(GetMutableDefaultPart()))
	{
		GetMutableDefaultPart()->DefaultShape.Add(PixelLocation);
	}
}

/**
 * Removes a pixel from the shape of the part this is curently editing.
 *
 * @param PixelLocation - The location of the pixel to remove.
 */
void UPartEditorBase::RemovePixelFromPartShape(FIntPoint PixelLocation)
{
	if (IsValid(GetMutableDefaultPart()))
	{
		GetMutableDefaultPart()->DefaultShape.Remove(PixelLocation);
	}
}

/**
 * Saves any changes made to the mutable default part.
 */
void UPartEditorBase::SavePart()
{
	if (IsValid(GetMutableDefaultPart()))
	{
		GetMutableDefaultPart()->UpdateDefaultConfigFile();
	}
}
