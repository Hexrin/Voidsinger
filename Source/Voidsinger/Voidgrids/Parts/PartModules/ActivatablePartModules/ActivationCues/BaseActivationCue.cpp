// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/BaseActivationCue.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"

/* \/ ================== \/ *\
|  \/ UBaseActivationCue \/  |
\* \/ ================== \/ */

UBaseActivationCue::UBaseActivationCue()
{
	UE_LOG(LogTemp, Warning, TEXT("i am constructed for some reason"));
}
/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 */
void UBaseActivationCue::Initialize(UActivatablePartModule* OwningModule)
{
	Module = OwningModule;
	Part = OwningModule->Part;
}

/* /\ Initialization /\ *\
\* -------------------- */

/* /\ ================== /\ *\
|  /\ UBaseActivationCue /\  |
\* /\ ================== /\ */