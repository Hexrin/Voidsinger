#if WITH_EDITOR

#include "PartEditorBase.h"
#include "Factories/DataAssetFactory.h"

/**
 * Gets the shape of the part this is curently editing.
 *
 * @return The locations of all the pixels of the shape.
 */
TArray<FIntPoint> UPartEditorBase::GetPartShape()
{
	if (IsValid(PartBeingEdited))
	{
		UE_LOG(LogTemp, Warning, TEXT("part being edited shape num %i"), PartBeingEdited->Shape.Num());
		return PartBeingEdited->Shape.Array();
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
	if (IsValid(PartBeingEdited) && IsValid(this))
	{
		PartBeingEdited->Shape = TSet<FIntPoint>(NewShape);
	}
}

/**
 * Adds a pixel to the shape of the part this is curently editing.
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
 * Removes a pixel from the shape of the part this is curently editing.
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
 * @return Whether or not the part was succesfuly saved.
 */
bool UPartEditorBase::SavePart()
{
	return UEditorAssetLibrary::SaveLoadedAsset(PartBeingEdited, true);
}
#endif