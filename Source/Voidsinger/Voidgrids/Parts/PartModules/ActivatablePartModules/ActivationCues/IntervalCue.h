// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivationCue.h"
#include "Tickable.h"
#include "IntervalCue.generated.h"

/* \/ ============ \/ *\
|  \/ UIntervalCue \/  |
\* \/ ============ \/ */

/**
 * Subclass of the base activation cue that is activated after every interval of time that passes
 */
UCLASS()
class VOIDSINGER_API UIntervalCue : public UBaseActivationCue, public FTickableGameObject
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

	/*
	 * Initializes this activation cue's variables and bindings
	 *
	 * @param OwningModule - The module that owns this activation cue
	 */
	virtual void Initialize(UActivatablePartModule* OwningModule) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------- *\
	\* \/ Tick \/ */

private:

	/**
	 * Tick override to track if the interval has passed and the delegate should be called
	 *
	 * @param DeltaTime - The time since the previous tick
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Determines if this object is tickable
	 *
	 * @return - Whether this object is tickable or not
	 */
	virtual bool IsTickable() const override;

	/**
	 * Returns this object's StatId
	 *
	 * @return - This object's StatId
	 */
	virtual TStatId GetStatId() const override;

	//Stores the time since the last tick interval
	float TimeSinceLastInterval;

	//Stores a reference to the owning part
	UPROPERTY()
	UPart* Part;

	UPROPERTY()
	UActivatablePartModule* Module;

	/* /\ Tick /\ *\
	\* ---------- */

	/* ---------------- *\
	\* \/ Delegation \/ */

public:

	//The time interval that the delegate should be called at. If this is less than the tick rate, then the interval will be every tick.
	UPROPERTY(EditAnywhere, Category = "Delegation")
	float Interval;

	/* /\ Delegation /\ *\
	\* ---------------- */
};

/* /\ ============ /\ *\
|  /\ UIntervalCue /\  |
\* /\ ============ /\ */