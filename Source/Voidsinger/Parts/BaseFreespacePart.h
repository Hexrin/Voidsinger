// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePart.h"
#include "BaseFreespacePart.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UBaseFreespacePart : public UBasePart
{
	GENERATED_BODY()

	
	void InitializeFunctionality();

	void InitializeVariables(FIntPoint Loc, float Rot, UPartGridComponent* PartGrid, TSubclassOf<UBasePart> PartType);

	void MergeParts(TSet<UBaseFreespacePart*> Other);
};
