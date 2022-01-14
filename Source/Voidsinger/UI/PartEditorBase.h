// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/PartData.h"
#include "EditorUtilityWidget.h"
#include "PartEditorBase.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UPartEditorBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	//Stores the part this is currently editing
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UPartData* PartBeingEdited;

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
