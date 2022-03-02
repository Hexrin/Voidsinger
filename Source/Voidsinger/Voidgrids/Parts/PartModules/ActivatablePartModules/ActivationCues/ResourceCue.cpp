// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceCue.h"
#include "BaseActivationCue.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"

/* \/ ============ \/ *\
|  \/ UResourceCue \/  |
\* \/ ============ \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 * 
 * @param OwningModule - The module that owns this activation cue
 */
void UResourceCue::Initialize(UActivatablePartModule* OwningModule)
{
	Part = OwningModule->Part;

	for (UBaseActivationCue* EachActivationCue : ActivationCues)
	{
		EachActivationCue->Initialize(OwningModule);
		EachActivationCue->OnActivate.AddUniqueDynamic(this, &UResourceCue::CreateResourceCall);
	}
}

/* /\ Initialization /\ *\
\* -------------------- */

/* --------------- *\
\* \/ Delegates \/ */

/*
 * Creates and uses the resources specified
 *
 * @param Data - The activation data containing all relavent information, including the effectiveness
 */
void UResourceCue::CreateResourceCall(FPartActivationData Data)
{
	if ((!bMustBeFunctional || Part->IsFunctional()))
	{
		PartActivationData = Data;

		//Stores the resource call adjusted with effectiveness
		FResourceCall AdjustedResourceCall;

		for (TPair<EResourceType, float> EachResourceTypeToAmountUsed : ResourceCall.ResourceTypesToAmountUsed)
		{
			AdjustedResourceCall.ResourceTypesToAmountUsed.Emplace(EachResourceTypeToAmountUsed.Key, EachResourceTypeToAmountUsed.Value * Data.Effectiveness);
		}

		for (TPair<EResourceType, float> EachResourceTypeToAmountCreated : ResourceCall.ResourceTypesToAmountCreated)
		{
			AdjustedResourceCall.ResourceTypesToAmountCreated.Emplace(EachResourceTypeToAmountCreated.Key, EachResourceTypeToAmountCreated.Value * Data.Effectiveness);
		}


		AdjustedResourceCall.OnResourceCallCompleted.AddDynamic(this, &UResourceCue::OnResourceCallCompleted);

		Part->GetVoidgrid()->AddResourceCall(AdjustedResourceCall);
	}
}

void UResourceCue::OnResourceCallCompleted()
{
	OnActivate.Broadcast(PartActivationData);
}

/* /\ Delegation /\ *\
\* ---------------- */

/* /\ ============ /\ *\
|  /\ UResourceCue /\  |
\* /\ ============ /\ */