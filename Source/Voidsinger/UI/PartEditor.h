// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Voidsinger/Voidgrids/BasePart.h"
#include "PartEditor.generated.h"

/**
 * A Widget for editing part defaults, shape, and resouce systems.
 */
UCLASS()
class VOIDSINGER_API UPartEditor : public UEditorUtilityWidgetBlueprint
{
	GENERATED_BODY()

public:
	//Stores the part this is currently editing
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UBasePart> PartBeingEdited;

	/**
	 * Gets the mutable default of the part this is curently editing.
	 * 
	 * @return The mutable default of the part this is curently editing.
	 */
	UFUNCTION(BlueprintPure)
	UBasePart* GetMutableDefaultPart();

	/**
	 * Gets the shape of the part this is curently editing.
	 * 
	 * @return The locations of all the pixels of the shape.
	 */
	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetPartShape();

	/**
	 * Sets the shape of the part this is curently editing.
	 *
	 * @param NewShape - The locations of all the pixels of the shape.
	 */
	UFUNCTION(BlueprintCallable)
	void SetPartShape(TArray<FIntPoint> NewShape);

	/**
	 * Adds a pixel to the shape of the part this is curently editing.
	 * 
	 * @param PixelLocation - The location of the pixel to add.
	 */
	UFUNCTION(BlueprintCallable)
	void AddPixelToPartShape(FIntPoint PixelLocation);

	/**
	 * Removes a pixel from the shape of the part this is curently editing.
	 *
	 * @param PixelLocation - The location of the pixel to remove.
	 */
	UFUNCTION(BlueprintCallable)
	void RemovePixelFromPartShape(FIntPoint PixelLocation);

	/**
	 * Saves any changes made to the mutable default part.
	 */
	UFUNCTION(BlueprintCallable)
	void SavePart();
};
