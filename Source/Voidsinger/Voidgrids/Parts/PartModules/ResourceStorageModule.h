// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/PartModule.h"
#include "Voidsinger/Voidgrids/ResourceType.h"
#include "ResourceStorageModule.generated.h"

/* \/ ====================== \/ *\
|  \/ UResourceStorageModule \/  |
\* \/ ====================== \/ */

/**
 * Subclass of the part module that adds storage capacity for resources
 */
UCLASS()
class VOIDSINGER_API UResourceStorageModule : public UPartModule
{
	GENERATED_BODY()

	/* ------------------- *\
	\* \/ Functionality \/ */
	
public:

	//Stores the amount of capacity for each resource this module adds
	UPROPERTY(EditAnywhere, Category = "Functionality")
	TMap<EResourceType, float> ResourceTypesToCapacityAmountsIncreased;

	/**
 	 * Adds storage capacity when the part is functional.
	 * 
	 * @param bApplyChangeEffect - Whether to apply the effects of this change
	 */
	virtual void FunctionalityRestored(bool bApplyChangeEffect) override;

	/**
	 * Removes the storage capacity this added when the part is not functional.
	 * 
	 * @param bApplyChangeEffect - Whether to apply the effects of this change
	 */
	virtual void FunctionalityLost(bool bApplyChangeEffect) override;

	/* /\ Functionality /\ *\
	\* ------------------- */
};

/* /\ ====================== /\ *\
|  /\ UResourceStorageModule /\  |
\* /\ ====================== /\ */