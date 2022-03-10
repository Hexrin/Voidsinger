// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartData.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityWidget.h"
#include "PartEditorBase.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGEREDITORTOOLS_API UPartEditorBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	//Stores the part this is currently editing
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPartData* PartBeingEdited;

	/**
	 * Gets the shape of the part this is currently editing.
	 * 
	 * @return The locations of all the pixels of the shape.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetPartShape();

	/**
	 * Sets the shape of the part this is currently editing.
	 *
	 * @param NewShape - The locations of all the pixels of the shape.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPartShape(TArray<FIntPoint> NewShape);

	/**
	 * Adds a pixel to the shape of the part this is currently editing.
	 * 
	 * @param PixelLocation - The location of the pixel to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddPixelToPartShape(FIntPoint PixelLocation);

	/**
	 * Removes a pixel from the shape of the part this is currently editing.
	 *
	 * @param PixelLocation - The location of the pixel to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemovePixelFromPartShape(FIntPoint PixelLocation);

	/**
	 * Saves the part this is editing
	 * 
	 * @return Whether or not the part was successfully saved.
	 */
	UFUNCTION(BlueprintCallable)
	bool SavePart();

	/**
	 * Creates a factory for making new parts.
	 */
	UFUNCTION(BlueprintPure)
	UFactory* CreatePartFactory();
};
#endif