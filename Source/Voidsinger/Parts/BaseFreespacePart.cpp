// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFreespacePart.h"
#include "PartGridComponent.h"



void UBaseFreespacePart::MergeParts(UBaseFreespacePart* Other)
{
	ActualShape += Other->GetShape();
	Other->DestroyPart();
}
