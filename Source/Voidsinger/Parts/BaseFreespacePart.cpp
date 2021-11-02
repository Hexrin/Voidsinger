// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFreespacePart.h"
#include "PartGridComponent.h"


void UBaseFreespacePart::InitializeFunctionality()
{
	Super::InitializeFunctionality();

	FPartGrid Grid = GetPartGrid()->GetPartGrid();
	TSet<UBaseFreespacePart*> PartsToMergeWith = TSet<UBaseFreespacePart*>();
	for (FIntPoint i = FIntPoint(1,0); i.Y != -1; i = (i * -1).X == 1 ? FIntPoint(0,1) : (i * -1))
	{
		UBaseFreespacePart* PartToMergeWith = Cast<UBaseFreespacePart>(Grid.Find(GetPartGridLocation() + i)->Part);
		if (PartToMergeWith)
		{
			PartsToMergeWith.Emplace(PartToMergeWith);
		}
	}
	MergeParts(PartsToMergeWith);
}

void UBaseFreespacePart::InitializeVariables(FIntPoint Loc, float Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType)
{
	DesiredShape = TSet<FIntPoint>();
	DesiredShape.Emplace(FIntPoint(0, 0));

	TArray<FIntPoint> ArrayToAdd = TArray<FIntPoint>();
	ArrayToAdd.Emplace(FIntPoint(1, 0));
	ArrayToAdd.Emplace(FIntPoint(-1, 0));
	ArrayToAdd.Emplace(FIntPoint(0, 1));
	ArrayToAdd.Emplace(FIntPoint(0, -1));
	for (auto& System : ResourceTypes)
	{
		ResourceTypes.Emplace(System.Key, ArrayToAdd);
	}

	Super::InitializeVariables(Loc, Rot, PartGrid, PartType);	
}

void UBaseFreespacePart::MergeParts(TSet<UBaseFreespacePart*> Others)
{
	for (auto& Other : Others)
	{
		ActualShape += Other->GetShape();
		Other->DestroyPart();
	}
}
