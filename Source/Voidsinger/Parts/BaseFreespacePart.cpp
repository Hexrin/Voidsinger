// Fill out your copyright notice in the Description page of Project Settings.

#include "PartGridComponent.h"
#include "BaseFreespacePart.h"

void UBaseFreespacePart::InitializeFunctionality()
{
	Super::InitializeFunctionality();

	FPartGrid Grid = GetPartGrid()->GetPartGrid();
	for (FIntPoint i = FIntPoint(1,0); i.Y != -1; i = (i * -1).X == 1 ? FIntPoint(0,1) : (i * -1))
	{
		FIntPoint CheckLoc = GetPartGridLocation() + i;
		if (Grid.Contains(CheckLoc) && Cast< UBaseFreespacePart>(Grid.Find(CheckLoc)->Part))
		{

		}
	}
}

void UBaseFreespacePart::InitializeVariables(FIntPoint Loc, float Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType)
{
	DesiredShape = TSet<FIntPoint>();
	DesiredShape.Emplace(FIntPoint(0, 0));
	Super::InitializeVariables(Loc, Rot, PartGrid, PartType);	
}