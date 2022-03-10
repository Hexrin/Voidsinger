// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Texture.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PartTooltipData.generated.h"

/**
 * Stores information relating to part tooltips.
 */
UCLASS()
class VOIDSINGER_API UPartTooltipData : public UDataAsset
{
	GENERATED_BODY()

public:

	//The name of the Part
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	//Describes what the part does and its stats.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	//The image used to show the connectivity points of this part.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ConectivityPointImage = nullptr;
};
