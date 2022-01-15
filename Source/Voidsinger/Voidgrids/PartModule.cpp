// Fill out your copyright notice in the Description page of Project Settings.


#include "PartModule.h"
#include"Part.h"


/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes the part module's private variables
 *
 * @param OwningPart - The part that this module is being initialized from
 */
void UPartModule::InitializeVariables(UPart* OwningPart)
{
	Part = OwningPart;
	BindToDelegates();
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ---------------- *\
\* \/ Activation \/ */

// \/ Activate \/

/**
 * Calls the "OnActivate" function so the part module's functionality is executed
 *
 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
 */
void UPartModule::Activate()
{
	TArray<TSubclassOf<UBaseVerbVoidsong>> EmptyVerbArray;
	OnActivate(EmptyVerbArray, 1);
}

/**
 * Activate overload - Checks whether "OnActivate" should be called by seeing if this module statisfies the Voidsong conditions. If it does, it calls the "OnActivate" function so the part module's functionality is executed
 * @param Factions - The Factions that were activated
 * @param Nouns - The Nouns that were activated
 * @param Verbs - The Verbs that were activated
 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
 */
void UPartModule::Activate(const TArray<EFactions>& Factions, const TArray<TSubclassOf<UPart>>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectiveness)
{
	//((Factions.IsEmpty() && AvailableFactions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) != Factions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) && NounsCheck
	
	TArray<EFactions> PlayableFactions;
	TArray <TSubclassOf<UPart>> PlayableNouns;

	for (TSubclassOf<UBaseVoidsong> EachPlayableVoidsong : PlayableVoidsongs)
	{
		if (EachPlayableVoidsong->IsChildOf(UBaseFactionVoidsong::StaticClass()))
		{
			PlayableFactions.Emplace(Cast<UBaseFactionVoidsong>(EachPlayableVoidsong->GetDefaultObject())->Faction);
		}
		if (EachPlayableVoidsong->IsChildOf(UBaseNounVoidsong::StaticClass()))
		{
			PlayableNouns.Emplace(Cast<UBaseNounVoidsong>(EachPlayableVoidsong->GetDefaultObject())->Noun);
		}
	}

	Part->GetVoidgrid();
	//bool bFactionsCheck = ((Factions.IsEmpty() && PlayableFactions.Contains(Part->GetVoidgrid())))
	//if (Factions.IsEmpty())
}

// /\ Activate /\

/* /\ Activation /\ *\
\* ---------------- */

/* ---------------------- *\
\* \/ Delegate Binding \/ */

void UPartModule::BindToDelegates()
{

	//switch (ActivationCues)

	//	case EActivationCue::OnDamaged:

			

}

/* /\ Delegate Binding /\ *\
\* ---------------------- */