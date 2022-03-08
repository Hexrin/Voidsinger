// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseVerbMotif.h"
#include "OrderVerbMotif.generated.h"

/* \/ =============== \/ *\
|  \/ UOrderVerbMotif \/  |
\* \/ =============== \/ */

/**
 * Repairs the ship hit with this motif
 */
UCLASS()
class VOIDSINGER_API UOrderVerbMotif : public UBaseVerbMotif
{
	GENERATED_BODY()

	/* ------------------------- *\
	\* \/ Voidsong Activation \/ */

public:

	/**
	 * Override of apply effect to apply the effect of this Motif
	 * 
	 * @param Instigator - The object that activated this Voidsong
	 * @param Hit - The hit result information
	 * @param Effectiveness - The effectiveness of the activation
	 */
	virtual void ApplyEffect(UObject* Instigator, FHitResult Hit, float Effectiveness) override;

	UPROPERTY(EditAnywhere, Category = "Voidsong Activation")
	float HealingRadius;

private:

	/**
	 * Gets all of the healable pixel locations on the Voidgrid given inside a given radius
	 *
	 * @param Voidgrid - The voidgrid to find pixels on
	 * @param Center - The center of the circle
	 * @param Radius - The radius
	 */
	UFUNCTION()
	TArray<FIntPoint> GetHealablePixelsInRadius(AVoidgrid* Voidgrid, FIntPoint Center, float Radius);

	/* /\ Voidsong Activation /\ *\
	\* ------------------------- */
};

/* /\ =============== /\ *\
|  /\ UOrderVerbMotif /\  |
\* /\ =============== /\ */