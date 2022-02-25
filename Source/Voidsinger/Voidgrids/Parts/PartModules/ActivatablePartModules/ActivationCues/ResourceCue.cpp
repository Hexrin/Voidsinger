// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseActivationCue.h"
#include "ResourceCue.h"

/* \/ ============ \/ *\
|  \/ UResourceCue \/  |
\* \/ ============ \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/*
 * Initializes this activation cue's variables and bindings
 */
void UResourceCue::Initialize(UPart* OwningPart)
{
	for (UBaseActivationCue* EachActivationCue : ActivationCues)
	{
		EachActivationCue->Initialize(OwningPart);
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