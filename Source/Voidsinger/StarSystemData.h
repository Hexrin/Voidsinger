// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoidsingerTypes.h"
#include "CoreMinimal.h"

/**
 * 
 */
class VOIDSINGER_API StarSystemData
{
public:
	StarSystemData();
	~StarSystemData();

	UPROPERTY(BlueprintReadOnly, ExposeOnSpawn = "true")
	FVector2D Location;

	UPROPERTY(BlueprintReadOnly, ExposeOnSpawn = "true")
	TEmumAsByte<EDifficulty> Difficulty;
};
