// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActivationCue.h"
#include "PartStateChangeCue.generated.h"

/* \/ ================ \/ *\
|  \/ EPartStateChange \/  |
\* \/ ================ \/ */

/**
 * This enum stores delegate names related to the part state
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EPartStateChange : uint8
{
	None						= 0			UMETA(Hidden),
	OnDamaged					= 1			UMETA(DisplayName = "OnDamaged", Tooltip = "When the part is damaged in any way"),
	OnRepaired					= 2			UMETA(DisplayName = "OnRepaired", Tooltip = "When the part is repaired in any way. This is different from fully repaired as you can repair a part one pixel at a time"),
	OnFunctionalityLost			= 4			UMETA(DisplayName = "OnFunctionalityLost", Tooltip = "When the part can no longer function"),
	OnFunctionalityRestored		= 8			UMETA(DisplayName = "OnFunctionalityRestored", Tooltip = "When the part gains enough pixels to function"),
	OnDestroyed					= 16		UMETA(DisplayName = "OnDestroyed", Tooltip = "When the part is completely destroyed"),
	OnFullyRepaired				= 32		UMETA(DisplayName = "OnFullyRepaired", Tooltip = "When the part becomes fully intact"),
};
ENUM_CLASS_FLAGS(EPartStateChange);

/* /\ ================ /\ *\
|  /\ EPartStateChange /\  |
\* /\ ================ /\ */

/* \/ =================== \/ *\
|  \/ UPartStateChangeCue \/  |
\* \/ =================== \/ */

/**
 * Subclass of the base activation cue that is activated when the part state is changed
 */
UCLASS()
class VOIDSINGER_API UPartStateChangeCue : public UBaseActivationCue
{
	GENERATED_BODY()
	
	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
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

	//Stores which part states to broadcast the delegate from
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Delegation", meta = (Bitmask, BitmaskEnum = EPartStateChange))
	int32 PartStateMask;

private:

	/**
	 * Broadcasts the "OnActivate" delegate
	 * 
	 * @param bApplyChangeEffect - Whether or not to actually call the delegate
	 */
	UFUNCTION()
	void BroadcastDelegate(bool bApplyChangeEffect);

	/* /\ Delegation /\ *\
	\* ---------------- */

};

/* /\ =================== /\ *\
|  /\ UPartStateChangeCue /\  |
\* /\ =================== /\ */