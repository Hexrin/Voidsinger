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

private:
	//Stores the Pixel Mold of this ship.
	PixelMoldType PixelMold;
};
