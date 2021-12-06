// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoidsingerTypes.h"
#include "CoreMinimal.h"
#include "StarSystemData.generated.h"

/**
 * 
 */

//Comment -Mabel
UCLASS(BlueprintType)
class VOIDSINGER_API UStarSystemData : public UObject
{

	GENERATED_BODY()

public:

	//Comment -Mabel
	UStarSystemData();

	//Comment -Mabel
	UPROPERTY(BlueprintReadOnly)
	FVector2D Location UMETA(ExposeOnSpawn = "true");

	//Comment -Mabel
	UPROPERTY(BlueprintReadOnly);
	EDifficulty Difficulty UMETA(ExposeOnSpawn = "true");
};
