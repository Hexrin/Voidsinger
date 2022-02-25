// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivationCue.h"
#include "ThrustRequestCue.generated.h"

/* \/ ================= \/ *\
|  \/ UThrustRequestCue \/  |
\* \/ ================= \/ */

/**
 * Subclass of the base activation cue that is activated when a thrust request is created
 */
UCLASS()
class VOIDSINGER_API UThrustRequestCue : public UBaseActivationCue
{
	GENERATED_BODY()
	
	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/*
	 * Initializes this activation cue's variables and bindings
	 */
	virtual void Initialize(UActivatablePartModule* OwningModule) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* --------------- *\
	\* \/ Delegates \/ */

private:

	/*
	 * Broadcasts the "OnActivate" delegate
	 *
	 * @param Data - The activation data containing the effectiveness, move direction, and rotation of the thrust request
	 */
	UFUNCTION()
	void BroadcastDelegate(FPartActivationData Data);

	/* /\ Delegates /\ *\
	\* --------------- */
};

/* /\ ================= /\ *\
|  /\ UThrustRequestCue /\  |
\* /\ ================= /\ */
