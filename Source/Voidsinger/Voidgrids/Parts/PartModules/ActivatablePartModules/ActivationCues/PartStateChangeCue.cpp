// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/PartStateChangeCue.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "PartStateChangeCue.h"

/* \/ =================== \/ *\
|  \/ UPartStateChangeCue \/  |
\* \/ =================== \/ */

/*
 * Initializes this activation cue's variables and bindings
 */
void UPartStateChangeCue::Initialize(UPart* OwningPart)
{
	// \/ Bind "BroadcastDelegate" to all part state changes specified \/ /

	if ((bool)(PartStates & EPartStateChange::OnDamaged))
	{
		OwningPart->OnDamaged.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStates & EPartStateChange::OnRepaired))
	{
		OwningPart->OnRepaired.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStates & EPartStateChange::OnFunctionalityLost))
	{
		OwningPart->OnFunctionalityLost.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStates & EPartStateChange::OnFunctionalityRestored))
	{
		OwningPart->OnFunctionalityRestored.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStates & EPartStateChange::OnDestroyed))
	{
		OwningPart->OnDestroyed.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}
	if ((bool)(PartStates & EPartStateChange::OnFullyRepaired))
	{
		OwningPart->OnFullyRepaired.AddDynamic(this, &UPartStateChangeCue::BroadcastDelegate);
	}

	// /\ Bind "BroadcastDelegate" to all part state changes specified /\ /
}

/*
 * Broadcasts the "OnActivate" delegate
 *
 * @param bApplyChangeEffect - Allows this function to bind to the part state changed delegates. The value is not used.
 */
void UPartStateChangeCue::BroadcastDelegate(bool bApplyChangeEffect)
{
	OnActivate.Broadcast(FPartActivationData());
}

/* /\ =================== /\ *\
|  /\ UPartStateChangeCue /\  |
\* /\ =================== /\ */