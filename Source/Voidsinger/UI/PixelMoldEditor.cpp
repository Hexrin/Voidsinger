// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PixelMoldEditor.h"

//Constucts a UPixelMoldEditor and initializes its save games.
UPixelMoldEditor::UPixelMoldEditor()
{
	MoldNamesSave = Cast<USlotNamesSave>(UGameplayStatics::LoadGameFromSlot(MoldNamesSaveSlotName, 0));
	if (!MoldNamesSave)
	{
		MoldNamesSave = Cast<USlotNamesSave>(UGameplayStatics::CreateSaveGameObject(USlotNamesSave::StaticClass()));
	}
}

/**
 * Sets the target's mold to the mold of this.
 */
void UPixelMoldEditor::ApplyMoldToTarget()
{
	Target->SetPixelMold(Mold);
}

/**
 * Sets the the mold of this to be the same as target's mold.
 */
void UPixelMoldEditor::LoadMoldFromTarget()
{
	Mold = Target->GetMinimalMoldData();
	OnMoldUpdated(Mold.Array());
}

/**
 * Saves the mold of this to a save slot of the given name.
 *
 * @param MoldName - The name of this mold and its save slot.
 */
void UPixelMoldEditor::SaveMold(FString MoldName)
{
	//Saves the molds data
	UPixelMoldSave* MoldSave = Cast<UPixelMoldSave>(UGameplayStatics::CreateSaveGameObject(UPixelMoldSave::StaticClass()));
	MoldSave->Data = Mold;
	UGameplayStatics::SaveGameToSlot(MoldSave, MoldSaveSlotNamePrefix.Append(MoldName), 0);

	//Saves the mold's existance
	MoldNamesSave->Names.Add(MoldName);
	UGameplayStatics::SaveGameToSlot(MoldNamesSave, MoldNamesSaveSlotName, 0);
}

/**
 * Loads the mold of a given name.
 *
 * @param MoldName - The name of the mold you want to load and its save slot.
 * @return Whether or not the mold was succesfuly loaded.
 */
bool UPixelMoldEditor::LoadMold(FString MoldName)
{
	UPixelMoldSave* MoldSave = Cast<UPixelMoldSave>(UGameplayStatics::LoadGameFromSlot(MoldSaveSlotNamePrefix.Append(MoldName), 0));
	if (MoldSave)
	{
		MoldSave->Data = Mold;
		return true;
	}
	return false;
}

/**
 * Gets whether the mold of a given name exists.
 *
 * @param MoldName - The name of the mold to search for.
 * @return Whether the mold of the given name exists.
 */
bool UPixelMoldEditor::DoesMoldExist(FString MoldName)
{
	return UGameplayStatics::DoesSaveGameExist(MoldSaveSlotNamePrefix.Append(MoldName), 0);
}

/**
 * Gets all of the names of the molds that the player has saved.
 *
 * @return An array of all mold names.
 */
UFUNCTION(BlueprintPure)
TArray<FString> UPixelMoldEditor::GetAllMoldNames()
{
	return MoldNamesSave->Names.Array();
}

/**
 * Adds a part to the mold of this.
 *
 * @param Part - The class of the part you want to add.
 * @param Transform - The location and rotation of the part you want to add.
 * @param OverrridePriorParts - Whether or not the part you are placing should override the parts it is placed on top of if possible.
 * @return Whether the part placement was a success.
 */
bool UPixelMoldEditor::PlacePart(TSubclassOf<UBasePart> Part, FPartTransform Transform, bool bOverrridePriorParts)
{
	TSet<FMinimalPartData> PartsToRemove = TSet<FMinimalPartData>();
	for (GridLocationType ShapeComponent : Cast<UBasePart>(Part->GetDefaultObject())->GetDefaultShape())
	{
		GridLocationType ShapeComonentGirdLocation = Transform.TransformGridLocation(ShapeComponent);
		if (PartLocations.Contains(ShapeComonentGirdLocation))
		{
			if (bOverrridePriorParts && Cast<UBasePart>(Part->GetDefaultObject())->bOverridable && Cast<UBasePart>(Part->GetDefaultObject())->bRemoveable)
			{
				PartsToRemove.Add(PartLocations.FindRef(ShapeComonentGirdLocation));
			}
			else
			{
				return false;
			}
		}
	}

	for (FMinimalPartData PartToRemove : PartsToRemove)
	{
		RemovePart(PartToRemove.Transform.GetGridLocation());
	}

	Mold.Add(FMinimalPartData(Part, Transform));
	for (GridLocationType ShapeComponent : Cast<UBasePart>(Part->GetDefaultObject())->GetDefaultShape())
	{
		PartLocations.Emplace(Transform.TransformGridLocation(ShapeComponent));
	}

	OnMoldUpdated(Mold.Array());
	return true;
}

// \/ RemovePart() \/

/**
 * Removes a part from the mold of this.
 *
 * @param Location - The location to remove a part from.
 * @return Wether or not a part was removed.
 */
bool UPixelMoldEditor::RemovePart(FIntPoint Location)
{
	return RemovePart(Location, true);
}


/**
 * Removes a part from the mold of this.
 *
 * @param Location - The location to remove a part from.
 * @param bCallUpdateEvent - Whether or not to call OnMoldUpdated() in this function.
 * @return Wether or not a part was removed.
 */
bool UPixelMoldEditor::RemovePart(FIntPoint Location, bool bCallUpdatedEvent)
{
	if (!PartLocations.Contains(Location))
	{
		return false;
	}

	if (bCallUpdatedEvent)
	{
		OnMoldUpdated(Mold.Array());
	}

	FMinimalPartData PartToRemove = PartLocations.FindRef(Location);
	for (GridLocationType ShapeComponent : Cast<UBasePart>(PartToRemove.Class->GetDefaultObject())->GetDefaultShape())
	{
		PartLocations.Remove(PartToRemove.Transform.TransformGridLocation(ShapeComponent));
	}
	Mold.Remove(PartToRemove);

	return true;
}

// /\ RemovePart() /\

/**
 * Gets the part data of the part at the given location.
 *
 * @param Location - The location to remove a part from.
 * @param PartData - Is set to the part data at Location.
 * @return Wether or not a part exists at Location.
 */
bool UPixelMoldEditor::GetPart(FIntPoint Location, FMinimalPartData& PartData)
{
	if (!PartLocations.Contains(Location))
	{
		return false;
	}

	PartData = PartLocations.FindRef(Location);
	return true;
}