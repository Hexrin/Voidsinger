// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidgrid.h"

// Sets default values
AVoidgrid::AVoidgrid()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


bool AVoidgrid::AddPart(TSubclassOf<UBasePart> PartClass, GridLocationType Location)
{
	PartClass.GetDefaultObject()->GetDesiredShape();
	return false;
}
