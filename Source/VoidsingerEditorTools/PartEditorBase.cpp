#if WITH_EDITOR

#include "PartEditorBase.h"
#include "Factories/DataAssetFactory.h"

/**
 * Gets the shape of the part this is currently editing.
 *
 * @return The locations of all the pixels of the shape.
 */
TArray<FIntPoint> UPartEditorBase::GetPartShape()
{
	if (IsValid(PartBeingEdited))
	{
		return PartBeingEdited->Shape.Array();
	}
	return TArray<FIntPoint>();
}

/**
 * Sets the shape of the part this is currently editing.
 *
 * @param NewShape - The locations of all the pixels of the shape.
 */
void UPartEditorBase::SetPartShape(TArray<FIntPoint> NewShape)
{
	if (IsValid(PartBeingEdited))
	{
		PartBeingEdited->Shape = TSet<FIntPoint>(NewShape);
	}
}

/**
 * Adds a pixel to the shape of the part this is currently editing.
 *
 * @param PixelLocation - The location of the pixel to add.
 */
void UPartEditorBase::AddPixelToPartShape(FIntPoint PixelLocation)
{
	if (IsValid(PartBeingEdited))
	{
		PartBeingEdited->Shape.Add(PixelLocation);
	}
}

/**
 * Removes a pixel from the shape of the part this is currently editing.
 *
 * @param PixelLocation - The location of the pixel to remove.
 */
void UPartEditorBase::RemovePixelFromPartShape(FIntPoint PixelLocation)
{
	if (IsValid(PartBeingEdited))
	{
		PartBeingEdited->Shape.Remove(PixelLocation);
	}
}

/**
 * Creates a factory for making new parts.
 */
UFactory* UPartEditorBase::CreatePartFactory()
{
	return NewObject<UDataAssetFactory>();
}

/**
 * Saves the part this is editing
 * 
 * @return Whether or not the part was successfully saved.
 */
bool UPartEditorBase::SavePart()
{
	return UEditorAssetLibrary::SaveLoadedAsset(PartBeingEdited, true);
}
#endif