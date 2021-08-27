// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Parts/BasePart.h"
#include "UObject/NoExportTypes.h"


USTRUCT(BlueprintType)
struct VOIDSINGER_API FPartData
{
	GENERATED_BODY()

		//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//class UBasePart* Part;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Temperature;
};