// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceCue.h"
#include "BaseActivationCue.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"

/* \/ ============ \/ *\
|  \/ UResourceCue \/  |
\* \/ ============ \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 */
void UResourceCue::Initialize(UActivatablePartModule* OwningModule)
{
	//Stores a reference to the owning part
	UPart* OwningPart = OwningModule->Part;

	Voidgrid = OwningPart->GetVoidgrid();

	for (UBaseActivationCue* EachActivationCue : ActivationCues)
	{
		EachActivationCue->Initialize(OwningModule);
		EachActivationCue->OnActivate.AddUniqueDynamic(this, &UResourceCue::ManageResources);
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
void UResourceCue::ManageResources(FPartActivationData Data)
{

}

/* /\ Delegates /\ *\
\* --------------- */

/* /\ ============ /\ *\
|  /\ UResourceCue /\  |
\* /\ ============ /\ */