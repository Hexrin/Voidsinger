// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/ThrustRequestCue.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"
#include "Voidsinger/Voidgrids/Voidgrid.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ThrustManager.h"

/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 */
void UThrustRequestCue::Initialize(UPart* OwningPart)
{
	OwningPart->GetVoidgrid()->ThrustManager->OnThrustRequest.AddDynamic(this, &UThrustRequestCue::BroadcastDelegate);
}

/* /\ Initialization /\ *\
\* -------------------- */

/* --------------- *\
\* \/ Delegates \/ */

/*
 * Broadcasts the "OnActivate" delegate
 *
 * @param Data - The activation data containing the effectiveness, move direction, and rotation of the thrust request
 */
void UThrustRequestCue::BroadcastDelegate(FPartActivationData Data)
{
	OnActivate.Broadcast(Data);
}

/* /\ Delegates /\ *\
\* --------------- */