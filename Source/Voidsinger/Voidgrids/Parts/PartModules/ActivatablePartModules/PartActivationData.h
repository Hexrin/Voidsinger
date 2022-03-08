// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/Voidsong.h"
#include "PartActivationData.generated.h"

/* \/ =================== \/ *\
|  \/ FPartActivationData \/  |
\* \/ =================== \/ */

USTRUCT(BlueprintType)
struct FPartActivationData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UBaseVerbMotif*> Verbs = TArray<UBaseVerbMotif*>();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Effectiveness{ 1 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Vector = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Rotation{ 0 };

	FPartActivationData(float ActivationEffectiveness = 1, FVector2D ActivationVector = FVector2D::ZeroVector, float ActivationRotation = 0, TArray<UBaseVerbMotif*> ActivationVerbs = TArray<UBaseVerbMotif*>())
	{
		Effectiveness = ActivationEffectiveness;
		Vector = ActivationVector;
		Rotation = ActivationRotation;
		Verbs = ActivationVerbs;
	}

	FPartActivationData(TArray<UBaseVerbMotif*> ActivationVerbs, float ActivationEffectiveness = 1, FVector2D ActivationVector = FVector2D::ZeroVector, float ActivationRotation = 0)
	{
		Effectiveness = ActivationEffectiveness;
		Vector = ActivationVector;
		Rotation = ActivationRotation;
		Verbs = ActivationVerbs;
	}
};

/* /\ =================== /\ *\
|  /\ FPartActivationData /\  |
\* /\ =================== /\ */
