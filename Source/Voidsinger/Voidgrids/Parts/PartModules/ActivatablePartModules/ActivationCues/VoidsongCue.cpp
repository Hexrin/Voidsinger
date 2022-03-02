// Fill out your copyright notice in the Description page of Project Settings.


#include "VoidsongCue.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "Voidsinger/Voidsongs/Voidsong.h"
#include "Voidsinger/VoidsingerGameMode.h"
#include "Kismet/GameplayStatics.h"

/* \/ ============ \/ *\
|  \/ UVoidsongCue \/  |
\* \/ ============ \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 * 
 * @param OwningModule - The module that owns this activation cue
 */
void UVoidsongCue::Initialize(UActivatablePartModule* OwningModule)
{
	Module = OwningModule;

	//Bind "BindToVoidsong" to Voidsong played
	Cast<AVoidsingerGameMode>(UGameplayStatics::GetGameMode(Module->GetWorld()))->VoidsongManager->OnVoidsongPlayed.AddDynamic(this, &UVoidsongCue::BindToVoidsong);
}

/* /\ Initialization /\ *\
\* -------------------- */

/* --------------- *\
\* \/ Delegates \/ */

void UVoidsongCue::BroadcastDelegate(const TArray<UBaseVerbMotif*> Verbs, float Effectiveness)
{
	OnActivate.Broadcast(FPartActivationData(Verbs, Effectiveness));
}

/**
 * Checks whether to bind to the Voidsong given by seeing if the owning module statisfies the Voidsong conditions. If it does, Activate is bound to the relavent VoidsongCues.
 *
 * @param Voidsong - The Voidsong to bind to
 */
void UVoidsongCue::BindToVoidsong(UVoidsong* Voidsong)
{
	//If the module's noun is not unbound, check if it satisfies the Voidsong requirements to activate.
	if (Module->Noun != ENoun::Unbound)
	{
		//Factions check is true if this faction is one of the factions played 
		bool bFactionsCheck = Voidsong->GetVoidsongData().GetFactions().Contains(Module->Part->GetVoidgrid()->GetFaction());

		//Nouns check is true if this noun is one of the nouns played
		bool bNounsCheck = Voidsong->GetVoidsongData().GetNouns().Contains(Module->Noun);

		//If this part module satisfies the conditions of the Voidsong, bind the events
		if (bFactionsCheck && bNounsCheck)
		{
			if ((bool)(VoidsongCueMask & (int)(EVoidsongCall::OnPlay)))
			{
				BroadcastDelegate(Voidsong->GetVoidsongData().GetVerbMotifs(), 1);
			}
			if ((bool)(VoidsongCueMask & (int)(EVoidsongCall::ForDuration)))
			{
				Voidsong->ForDuration.AddDynamic(this, &UVoidsongCue::BroadcastDelegate);
			}
			if ((bool)(VoidsongCueMask & (int)(EVoidsongCall::OnBeat)))
			{
				Voidsong->OnBeat.AddDynamic(this, &UVoidsongCue::BroadcastDelegate);
			}
		}
	}
}

/* /\ Delegation /\ *\
\* ---------------- */

/* /\ ============ /\ *\
|  /\ UVoidsongCue /\  |
\* /\ ============ /\ */