// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Voidsinger/Voidgrids/Ships/Ship.h"
#include "GameFramework/SaveGame.h"
#include "PixelMoldEditorBase.generated.h"

/**
 * A widget to editing, saveing and loading pixel molds
 */
UCLASS()
class VOIDSINGER_API UPixelMoldEditorBase : public UUserWidget
{
	GENERATED_BODY()

public:
	//Initilize the save game 
	virtual void NativeOnInitialized() override;
protected:
	//Stores the Voidgrid this is editing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AShip* Target;

	/**
	 * Sets the target's mold to the mold of this.
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyMoldToTarget();

	/**
	 * Sets the mold this is editing.
	 * 
	 * @param NewMold - The new mold to edit.
	 */
	UFUNCTION(BlueprintCallable)
	void SetMold(TArray<FMinimalPartInstanceData> NewMold);
	void SetMold(MinimalPixelMoldDataType NewMold);

	/**
	 * Sets the the mold of this to be the same as target's mold.
	 */
	UFUNCTION(BlueprintCallable)
	void LoadMoldFromTarget();

	/**
	 * Saves the mold of this to a save slot of the given name.
	 * 
	 * @param MoldName - The name of this mold and its save slot.
	 */
	UFUNCTION(BlueprintCallable)
	void SaveMold(FString MoldName);

	/**
	 * Loads the mold of a given name.
	 *
	 * @param MoldName - The name of the mold you want to load and its save slot.
	 * @return Whether or not the mold was succesfuly loaded.
	 */
	UFUNCTION(BlueprintCallable)
	bool LoadMold(FString MoldName);

	/**
	 * Gets whether the mold of a given name exists.
	 * 
	 * @param MoldName - The name of the mold to search for.
	 * @return Whether the mold of the given name exists.
	 */
	UFUNCTION(BlueprintPure)
	bool DoesMoldExist(FString MoldName);

	/**
	 * Gets all of the names of the molds that the player has saved.
	 * 
	 * @return An array of all mold names.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FString> GetAllMoldNames();

	/**
	 * Adds a part to the mold of this.
	 * 
	 * @param Part - The class of the part you want to add.
	 * @param Transform - The location and rotation of the part you want to add.
	 * @param OverrridePriorParts - Whether or not the part you are placing should override the parts it is placed on top of if possible.
	 * @return Whether the part placement was a success.
	 */
	UFUNCTION(BlueprintCallable)
	bool PlacePart(UPartData* Part, FPartTransform Transform, bool bOverrridePriorParts = false, bool bFreeBuildMode = false);


	// \/ RemovePart() \/ /

	/**
	 * Removes a part from the mold of this.
	 * 
	 * @param Location - The location to remove a part from.
	 * @return Wether or not a part was removed.
	 */
	UFUNCTION(BlueprintCallable)
	bool RemovePart(FIntPoint Location);

	/**
	 * Removes a part from the mold of this.
	 *
	 * @param Location - The location to remove a part from.
	 * @param bCallUpdateEvent - Whether or not to call OnMoldUpdated() in this function.
	 * @return Wether or not a part was removed.
	 */
	bool RemovePart(FIntPoint Location, bool bCallUpdateEvent);

	// /\ RemovePart() /\ /

	/**
	 * Gets the part data of the part at the given location.
	 *
	 * @param Location - The location to remove a part from.
	 * @param PartData - Is set to the part data at Location.
	 * @return Wether or not a part exists at Location.
	 */
	UFUNCTION(BlueprintCallable)
	bool GetPart(FIntPoint Location, FMinimalPartInstanceData& PartData);

	/**
	 * Called when the mold of this is changed in any way.
	 * 
	 * @param NewMold - The mold of this.
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnMoldUpdated(const TArray<FMinimalPartInstanceData>& NewMold, const TArray<FMinimalPartInstanceData>& ChangedData, bool bDataRemoved);

private:
	//Stores the mold of this.
	MinimalPixelMoldDataType Mold;

	//Stores the locations of all parts in the mold of this.
	TGridMap<FMinimalPartInstanceData> PartLocations;

	//Stores the prefix of all molds saved by this.
	FString MoldSaveSlotNamePrefix = TEXT("Mold_");

	//Stores the slot name of for the MoldNamesSave.
	FString MoldNamesSaveSlotName = TEXT("MoldNames");

	//Stores a pointer to a save game storing all of the names of molds that the player has saved.
	USlotNamesSave* MoldNamesSave;
};

/**
 * A save game class for storing a LocationsToPixelState
 */
UCLASS()
class VOIDSINGER_API UPixelMoldSave : public USaveGame
{
	GENERATED_BODY()

public:
	//Stores a minimal mold data
	MinimalPixelMoldDataType Data;
};

/**
 * A save game class for storing other save games
 */
UCLASS()
class VOIDSINGER_API USlotNamesSave : public USaveGame
{
	GENERATED_BODY()

public:
	//Stores a minimal mold data
	UPROPERTY()
	TSet<FString> Names;
};