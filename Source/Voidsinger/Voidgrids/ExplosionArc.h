// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplosionArc.generated.h"

USTRUCT()
struct VOIDSINGER_API FExplosionArc
{
	GENERATED_BODY()

	UPROPERTY()
	float LowerArcYValue{ 0 };

	UPROPERTY()
	bool bLowerArcNegative{ false };

	UPROPERTY()
	float UpperArcYValue{ 0 };

	UPROPERTY()
	bool bUpperArcNegative{ true };
};