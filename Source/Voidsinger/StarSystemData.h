// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoidsingerTypes.h"
#include "CoreMinimal.h"
#include "StarSystemData.generated.h"

/**
 * 
 */

//Comment -Mabel Suggestion
UCLASS(BlueprintType)
class VOIDSINGER_API UStarSystemData : public UObject
{

	GENERATED_BODY()

public:

	//Comment -Mabel Suggestion
	UStarSystemData();

	//Comment -Mabel Suggestion
	UPROPERTY(BlueprintReadOnly)
	FVector2D Location UMETA(ExposeOnSpawn = "true");

	//Comment -Mabel Suggestion
	UPROPERTY(BlueprintReadOnly);
	TEnumAsByte<EDifficulty> Difficulty UMETA(ExposeOnSpawn = "true");
};
