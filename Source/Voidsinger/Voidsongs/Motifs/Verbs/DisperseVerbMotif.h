// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseVerbMotif.h"
#include "DisperseVerbMotif.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UDisperseVerbMotif : public UBaseVerbMotif
{
	GENERATED_BODY()
	
	virtual void ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness) override;
};
