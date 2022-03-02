// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/PartStateChangeCue.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"
#include "PartStateChangeCue.h"

/* \/ =================== \/ *\
|  \/ UPartStateChangeCue \/  |
\* \/ =================== \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 * 
 * @param OwningModule - The module that owns this activation cue
 */
void UPartStateChangeCue::Initialize(UActivatablePartModule* OwningModule)
{
	//Stores a reference to the owning part
	UPart* OwningPart = OwningModule->Part;

	// \/ Bind "BroadcastDelegate" to all part state changes specified \/ /

	if ((bool)(PartStateMask & (int)(EPartStateChange::OnDamaged)))
	{
		OwningPart->OnDamaged.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStateMask & (int)(EPartStateChange::OnRepaired)))
	{
		OwningPart->OnRepaired.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStateMask & (int)(EPartStateChange::OnFunctionalityLost)))
	{
		OwningPart->OnFunctionalityLost.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStateMask & (int)(EPartStateChange::OnFunctionalityRestored)))
	{
		OwningPart->OnFunctionalityRestored.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStateMask & (int)(EPartStateChange::OnDestroyed)))
	{
		OwningPart->OnDestroyed.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStateMask & (int)(EPartStateChange::OnFullyRepaired)))
	{
		OwningPart->OnFullyRepaired.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}

	// /\ Bind "BroadcastDelegate" to all part state changes specified /\ /
}

/* /\ Initialization /\ *\
\* -------------------- */

/* --------------- *\
\* \/ Delegates \/ */

/*
 * Broadcasts the "OnActivate" delegate
 *
 * @param bApplyChangeEffect - Whether or not to actually call the delegate
 */
void UPartStateChangeCue::BroadcastDelegate(bool bApplyChangeEffect)
{
	if (bApplyChangeEffect)
	{
		OnActivate.Broadcast(FPartActivationData());
	}
}

/* /\ Delegation /\ *\
\* ---------------- */

/* /\ =================== /\ *\
|  /\ UPartStateChangeCue /\  |
\* /\ =================== /\ */