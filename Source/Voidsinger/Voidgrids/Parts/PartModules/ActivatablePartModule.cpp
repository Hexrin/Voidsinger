// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "Voidsinger/StarSystemGameMode.h"
#include "Kismet/GameplayStatics.h"

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
 * Calls the "ActivateWithEffectiveness" function with an Effectiveness of 1 so the part module's functionality is executed.
 */
void UActivatablePartModule::Activate()
{
	ActivateWithEffectiveness(1);
}

/**
 * Calls the "OnActivate" function so the part module's functionality is executed
 *
 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
 */
void UActivatablePartModule::ActivateWithEffectiveness(float Effectiveness)
{
	TArray<UBaseVerbMotif*> EmptyVerbArray;
	OnActivate(EmptyVerbArray, Effectiveness);
}

/**
 * Calls the "OnActivate" function with the Verbs played in a Voidsong so the part module's functionality of executed
 *
 * @param Verbs - The Verbs played in the Voidsong
 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
 */
void UActivatablePartModule::ActivateFromVoidsong(const TArray<UBaseVerbMotif*> Verbs, float Effectiveness)
{
	OnActivate(Verbs, Effectiveness);
}

// /\ Activate /\

/* /\ Activation /\ *\
\* ---------------- */

/* ---------------------- *\
\* \/ Delegate Binding \/ */

void UActivatablePartModule::BindToDelegates()
{
	//Bind "BindToVoidsong" to Voidsong played
	Cast<AStarSystemGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->VoidsongManager->OnVoidsongPlayed.AddDynamic(this, &UActivatablePartModule::BindToVoidsong);

	// \/ Bind Activate to the Activation Cues selected \/

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
		//Bind to on tick here (Delegate is not available yet)
	}

	// /\ Bind Activate to the Activation Cues selected /\

}

/**
 * Checks whether to bind to the Voidsong given by seeing if this module statisfies the Voidsong conditions. If it does, ActivateWithEffectiveness is bound to the relavent VoidsongCues.
 *
 * @param Voidsong - The Voidsong to bind to
 */
void UActivatablePartModule::BindToVoidsong(UVoidsong* Voidsong)
{
	//If this module's noun is not unbound, check if it satisfies the Voidsong requirements to activate. Also check if this should be bound to a VoidsongCue.
	if (Noun != ENoun::Unbound && (bool)(ActivationCues & EActivationCue::OnVoidsongCue))
	{
		//Factions check is true if this faction is one of the factions played 
		bool bFactionsCheck = Voidsong->VoidsongData.GetFactions().Contains(Part->GetVoidgrid()->GetFaction());

		//Nouns check is true if this noun is one of the nouns played
		bool bNounsCheck = Voidsong->VoidsongData.GetNouns().Contains(Noun);

		//If this part module satisfies the conditions of the Voidsong, bind the events
		if (bFactionsCheck && bNounsCheck)
		{
			if ((bool)(VoidsongCues & EVoidsongCue::ForDuration))
			{
				Voidsong->ForDuration.AddDynamic(this, &UActivatablePartModule::ActivateFromVoidsong);
			}
			if ((bool)(VoidsongCues & EVoidsongCue::OnBeat))
			{
				Voidsong->OnBeat.AddDynamic(this, &UActivatablePartModule::ActivateFromVoidsong);
			}
		}
	}
}

/* /\ Delegate Binding /\ *\
\* ---------------------- */