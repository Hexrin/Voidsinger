// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PartModule.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable)
class VOIDSINGER_API UPartModule : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Range;
};
