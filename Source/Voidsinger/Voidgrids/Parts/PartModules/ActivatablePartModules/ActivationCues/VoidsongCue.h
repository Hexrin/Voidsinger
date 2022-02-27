// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivationCue.h"
#include "VoidsongCue.generated.h"

// \/ Forward declarations \/ /

class UVoidsong;

// /\ Forward declarations /\ /

/* \/ ============ \/ *\
|  \/ EVoidsongCall \/  |
\* \/ ============ \/ */

/**
 * This enum stores delegate names specific to Voidsongs
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EVoidsongCall : uint8
{
	None			= 0		UMETA(Hidden),
	OnPlay			= 1		UMETA(DisplayName = "On Play", Tooltip = "When the Voidsong begins playing"),
	OnBeat			= 2		UMETA(DisplayName = "On Beat", Tooltip = "On the beat of the Voidsong"),
	ForDuration		= 4		UMETA(DisplayName = "For Duration", Tooltip = "Called every tick for the duration of the Voidsong"),
};
ENUM_CLASS_FLAGS(EVoidsongCall)

/* /\ ============ /\ *\
|  /\ EVoidsongCall /\  |
\* /\ ============ /\ */

/* \/ ============ \/ *\
|  \/ UVoidsongCue \/  |
\* \/ ============ \/ */

/**
 * Subclass of the base activation cue that is activated when a Voidsong is played
 */
UCLASS()
class VOIDSINGER_API UVoidsongCue : public UBaseActivationCue
{

	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/*
	 * Initializes this activation cue's variables and bindings
	 * 
	 * @param OwningModule - The module that owns this activation cue
	 */
	virtual void Initialize(UActivatablePartModule* OwningModule) override;

	/* /\ Initialization /\ *\
	\* -------------------- */
	
	/* ---------------- *\
	\* \/ Delegation \/ */

public:

	//Stores which Voidsong cues to broadcast the delegate from
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Voidsong Cue", meta = (Bitmask, BitmaskEnum = EVoidsongCall))
	int32 VoidsongCueMask;

private:

	/*
	 * Broadcasts the "OnActivate" delegate
	 *
	 * @param Verbs - The verbs played in the Voidsong
	 * @param Effectiveness - The effectiveness of the activation
	 */
	UFUNCTION()
	void BroadcastDelegate(const TArray<UBaseVerbMotif*> Verbs, float Effectiveness);

	/**
	 * Checks whether to bind to the Voidsong given by seeing if this module statisfies the Voidsong conditions. If it does, Activate is bound to the relavent VoidsongCues.
	 *
	 * @param Voidsong - The Voidsong to bind to
	 */
	UFUNCTION()
	void BindToVoidsong(UVoidsong* Voidsong);

	//Stores a reference to the owning module
	UActivatablePartModule* Module;

	/* /\ Delegation /\ *\
	\* ---------------- */
};

/* /\ ============ /\ *\
|  /\ UVoidsongCue /\  |
\* /\ ============ /\ */
