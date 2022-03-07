// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceCreationModule.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* \/ ======================= \/ *\
|  \/ UResourceCreationModule \/  |
\* \/ ======================= \/ */

/* ---------------- *\
\* \/ Activation \/ */

/**
 * Override of OnActivate to create resources
 *
 * @param Data - The data of this activation
 */
void UResourceCreationModule::OnActivate_Implementation(const FPartActivationData Data)
{
	// \/ Adjust the amount of resources created with effectiveness \/ //

	TMap<EResourceType, float> AdjustedResourceTypesToAmountsCreated;

	for (TPair<EResourceType, float> EachResourceTypeToAmountCreated : ResourceTypesToAmountsCreated)
	{
		AdjustedResourceTypesToAmountsCreated.Emplace(EachResourceTypeToAmountCreated.Key, EachResourceTypeToAmountCreated.Value * Data.Effectiveness);
	}

	// /\ Adjust the amount of resources created with effectiveness /\ //

	GetVoidgrid()->AddResources(AdjustedResourceTypesToAmountsCreated);
}

/* /\ Activation /\ *\
\* ---------------- */

/* /\ ======================= /\ *\
|  /\ UResourceCreationModule /\  |
\* /\ ======================= /\ */