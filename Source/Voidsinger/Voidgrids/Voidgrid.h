// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Voidsinger/Parts/BasePart.h"
#include "GridMap.h"
#include "Voidgrid.generated.h"

UCLASS()
class VOIDSINGER_API AVoidgrid : public APawn
{
	GENERATED_BODY()

public:
	//Sets default values for this voidgrid's properties
	AVoidgrid();

	/* ---------------- *\ 
	\* \/ Pixel Mold \/ */
public:
	//The type used for storing a ships Pixel Mold
	typedef TGridMap<UBasePart*> PixelMoldType;

	/**
	 * Adds a part to this ships Pixel Mold.
	 * 
	 * @param PartClass - The class of part to add.
	 * @param Location - The location to add the part at.
	 * @return Whether or not a part was placed successfuly.
	 */
	bool AddPart(TSubclassOf<UBasePart> PartClass, GridLocationType Location);

private:
	//Stores the Pixel Mold of this ship.
	PixelMoldType PixelMold;
};
