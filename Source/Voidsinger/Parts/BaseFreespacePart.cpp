// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFreespacePart.h"
#include "PartGridComponent.h"



void UBaseFreespacePart::MergeParts(UBaseFreespacePart* Other)
{
	FIntPoint RelativeLoc = Other->GetPartGridLocation() - GetPartGridLocation();
	for (FIntPoint PixelLoc : Other->GetShape())
	{
		ActualShape.Emplace(PixelLoc + RelativeLoc);
	}
	Other->DestroyPart();
}
