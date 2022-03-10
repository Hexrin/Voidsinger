// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/ResourceType.h"
#include "ResourceCreationModule.generated.h"

/* \/ ======================= \/ *\
|  \/ UResourceCreationModule \/  |
\* \/ ======================= \/ */

/**
 * Subclass of the activatable part module that can create resources
 */
UCLASS()
class VOIDSINGER_API UResourceCreationModule : public UActivatablePartModule
{
	GENERATED_BODY()
	
	/* ---------------- *\
	\* \/ Activation \/ */

public:

	//The resource types that this module creates and how much of each it creates
	UPROPERTY(EditAnywhere, Category = "Activation")
	TMap<EResourceType, float> ResourceTypesToAmountsCreated;

protected:

	/**
	 * Override of OnActivate to create resources
	 * 
	 * @param Data - The data of this activation
	 */
	virtual void OnActivate_Implementation(const FPartActivationData Data) override;

	/* /\ Activation /\ *\
	\* ---------------- */
};

/* /\ ======================= /\ *\
|  /\ UResourceCreationModule /\  |
\* /\ ======================= /\ */