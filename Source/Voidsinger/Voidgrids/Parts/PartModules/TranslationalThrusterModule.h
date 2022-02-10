// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseThrusterModule.h"
#include "TranslationalThrusterModule.generated.h"

/**
 * 
 */
UCLASS()
class VOIDSINGER_API UTranslationalThrusterModule : public UBaseThrusterModule
{
	GENERATED_BODY()

protected:
	/**
	 * Called when the owning part is activated and calls the functionality of this part.
	 * Applies an impulse to the voidgrid in an atempt to get it to move in direction.
	 */
	virtual void OnActivate_Implementation(const TArray<UBaseVerbMotif*>& Verbs, float Effectiveness, FVector2D Direction = FVector2D::ZeroVector) override;
};
