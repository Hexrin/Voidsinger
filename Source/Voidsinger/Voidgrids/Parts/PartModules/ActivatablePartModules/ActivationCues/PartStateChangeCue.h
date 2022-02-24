// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidgrids/Parts/PartModules/ActivatablePartModules/ActivationCues/BaseActivationCue.h"
#include "PartStateChangeCue.generated.h"

/* \/ ================ \/ *\
|  \/ EPartStateChange \/  |
\* \/ ================ \/ */

/**
 * This enum stores delegate names related to the part state.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EPartStateChange : uint8
{
	OnDamaged					UMETA(DisplayName = "OnDamaged", Tooltip = "When the part is damaged in any way"),
	OnRepaired					UMETA(DisplayName = "OnRepaired", Tooltip = "When the part is repaired in any way. This is different from fully repaired as you can repair a part one pixel at a time"),
	OnFunctionalityLost			UMETA(DisplayName = "OnFunctionalityLost", Tooltip = "When the part can no longer function"),
	OnFunctionalityRestored		UMETA(DisplayName = "OnFunctionalityRestored", Tooltip = "When the part gains enough pixels to function"),
	OnDestroyed					UMETA(DisplayName = "OnDestroyed", Tooltip = "When the part is completely destroyed"),
	OnFullyRepaired				UMETA(DisplayName = "OnFullyRepaired", Tooltip = "When the part becomes fully intact"),
};
ENUM_CLASS_FLAGS(EPartStateChange);

/* /\ ================ /\ *\
|  /\ EPartStateChange /\  |
\* /\ ================ /\ */

/* \/ =================== \/ *\
|  \/ UPartStateChangeCue \/  |
\* \/ =================== \/ */

/**
 * Subclass of the base activation cue that is activated when the part state changed
 */
UCLASS()
class VOIDSINGER_API UPartStateChangeCue : public UBaseActivationCue
{
	GENERATED_BODY()
	
public:

	/*
	 * Initializes this activation cue's variables and bindings
	 */
	virtual void Initialize(UPart* OwningPart) override;

private:

	/*
	 * Broadcasts the "OnActivate" delegate
	 * 
	 * @param bApplyChangeEffect - Allows this function to bind to the part state changed delegates. The value is not used.
	 */
	void BroadcastDelegate(bool bApplyChangeEffect);

public:

	//Stores which part states to broadcast the delegate from
	UPROPERTY(BlueprintReadWrite)
	EPartStateChange PartStates;

};

/* /\ =================== /\ *\
|  /\ UPartStateChangeCue /\  |
\* /\ =================== /\ */