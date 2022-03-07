// Fill out your copyright notice in the Description page of Project Settings.


#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/IntervalCue.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivatablePartModule.h"
#include "Voidsinger/Voidgrids/Parts/Part.h"

/* \/ ============ \/ *\
|  \/ UIntervalCue \/  |
\* \/ ============ \/ */

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes this activation cue's variables and bindings
 *
 * @param OwningModule - The module that owns this activation cue
 */
void UIntervalCue::Initialize(UActivatablePartModule* OwningModule)
{
	Module = OwningModule;
	Part = OwningModule->Part;
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ---------- *\
\* \/ Tick \/ */

/**
 * Tick override to track if the interval has passed and the delegate should be called
 *
 * @param DeltaTime - The time since the previous tick
 */
void UIntervalCue::Tick(float DeltaTime)
{
	TimeSinceLastInterval += DeltaTime;

	if (TimeSinceLastInterval > Interval)
	{
		OnActivate.Broadcast(FPartActivationData(TimeSinceLastInterval));
		TimeSinceLastInterval = 0;
	}
}

/**
 * Determines if this object is tickable
 *
 * @return - Whether this object is tickable or not
 */
bool UIntervalCue::IsTickable() const
{
	return (!IsTemplate(RF_ClassDefaultObject) && IsValid(Part) && Part->HasAnyPixelsIntact() && (!bMustBeFunctional || Part->IsFunctional()));
}

/**
 * Returns this object's StatId
 *
 * @return - This object's StatId
 */
TStatId UIntervalCue::GetStatId() const
{
	return TStatId();
}

/* /\ Tick /\ *\
\* ---------- */

/* /\ ============ /\ *\
|  /\ UIntervalCue /\  |
\* /\ ============ /\ */