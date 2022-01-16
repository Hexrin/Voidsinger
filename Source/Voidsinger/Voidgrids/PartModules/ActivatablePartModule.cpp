// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes the part module's protected and private variables. Overriden from PartModule to call BindToDelegates().
 *
 * @param OwningPart - The part that this module is being initialized from
 */
void UActivatablePartModule::InitializeVariables(UPart* OwningPart)
{
	Super::InitializeVariables(OwningPart);

	BindToDelegates();
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ---------------- *\
\* \/ Activation \/ */

// \/ Activate \/

/**
 * Calls the "OnActivate" function with an Effectiveness of 1 so the part module's functionality is executed.
 */
void UActivatablePartModule::Activate()
{
	TArray<TSubclassOf<UBaseVerbVoidsong>> EmptyVerbArray;
	OnActivate(EmptyVerbArray, 1);
}

/**
 * Calls the "OnActivate" function so the part module's functionality is executed
 *
 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
 */
void UActivatablePartModule::ActivateWithEffectiveness(float Effectiveness)
{
	TArray<TSubclassOf<UBaseVerbVoidsong>> EmptyVerbArray;
	OnActivate(EmptyVerbArray, Effectiveness);
}

/**
 * Activate overload - Checks whether "OnActivate" should be called by seeing if this module statisfies the Voidsong conditions. If it does, it calls the "OnActivate" function so the part module's functionality is executed
 *
 * @param Factions - The Factions that were activated
 * @param Nouns - The Nouns that were activated
 * @param Verbs - The Verbs that were activated
 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
 */
void UActivatablePartModule::ActivateFromVoidsong(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectiveness)
{
	//If this module's noun is not unbound, check if it satisfies the Voidsong requirements to activate
	if (Noun != ENoun::Unbound)
	{

		//((Factions.IsEmpty() && AvailableFactions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) != Factions.Contains(Cast<ABaseShip>(GetOuter()->GetOuter())->GetFaction())) && NounsCheck

		// \/ Finds playable Factions  and Nouns from all playable Voidsongs \/

		TArray<EFaction> PlayableFactions;
		TArray<ENoun> PlayableNouns;

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

		// /\ Finds playable Factions and Nouns from all playable Voidsongs /\

		//Checks if this module's faction (referred to as "this faction") is playable
		bool bFactionIsPlayable = PlayableFactions.Contains(Part->GetVoidgrid()->GetFaction());

		//Factions check is true if 
		//	      |- No factions played, but this faction is playable -| or |--- This faction is one of the factions played ---|
		bool bFactionsCheck = (Factions.IsEmpty() && bFactionIsPlayable) || Factions.Contains(Part->GetVoidgrid()->GetFaction());

		//Checks is this module's noun (referred to as "this noun") is playable
		bool bNounIsPlayable = PlayableNouns.Contains(Noun);

		//Nouns check is true if
		//	    |- No noun played, but this noun is playable -| or |- This noun is one of the nouns played -|
		bool bNounsCheck = (Nouns.IsEmpty() && bNounIsPlayable) || Nouns.Contains(Noun);

		//If this part module satisfies the conditions of the Voidsong, call OnActivate()
		if (bFactionsCheck && bNounsCheck)
		{
			OnActivate(Verbs, Effectiveness);
		}
	}
}

// /\ Activate /\

/* /\ Activation /\ *\
\* ---------------- */

/* ---------------------- *\
\* \/ Delegate Binding \/ */

void UActivatablePartModule::BindToDelegates()
{
	if ((bool)(ActivationCues & EActivationCue::OnDamaged))
	{
		Part->OnDamaged.AddDynamic(this, &UActivatablePartModule::Activate);
	}
	if ((bool)(ActivationCues & EActivationCue::OnRepaired))
	{
		Part->OnRepaired.AddDynamic(this, &UActivatablePartModule::Activate);
	}
	if ((bool)(ActivationCues & EActivationCue::OnFunctionalityLost))
	{
		Part->OnFunctionalityLost.AddDynamic(this, &UActivatablePartModule::Activate);
	}
	if ((bool)(ActivationCues & EActivationCue::OnFunctionalityRestored))
	{
		Part->OnFunctionalityRestored.AddDynamic(this, &UActivatablePartModule::Activate);
	}
	if ((bool)(ActivationCues & EActivationCue::OnDestroyed))
	{
		Part->OnDestroyed.AddDynamic(this, &UActivatablePartModule::Activate);
	}
	if ((bool)(ActivationCues & EActivationCue::OnFullyRepaired))
	{
		Part->OnFullyRepaired.AddDynamic(this, &UActivatablePartModule::Activate);
	}
	if ((bool)(ActivationCues & EActivationCue::OnTick))
	{

	}
}

/* /\ Delegate Binding /\ *\
\* ---------------------- */