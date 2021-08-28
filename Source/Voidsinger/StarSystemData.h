// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoidsingerTypes.h"
#include "CoreMinimal.h"
#include "StarSystemData.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UStarSystemData : public UObject
{

	GENERATED_BODY()

public:
	UStarSystemData();

	UPROPERTY(BlueprintReadOnly)
	FVector2D Location UMETA(ExposeOnSpawn = "true");

	UPROPERTY(BlueprintReadOnly);
	TEnumAsByte<EDifficulty> Difficulty UMETA(ExposeOnSpawn = "true");
};
