// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/Voidsongs/Factions/BaseFactionVoidsong.h"
#include "Voidsinger/Voidsongs/Nouns/BaseNounVoidsong.h"
#include "Voidsinger/Voidsongs/Verbs/BaseVerbVoidsong.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "PartModule.generated.h"

/* \/ ============== \/ *\
|  \/ EActivationCue \/  |
\* \/ ============== \/ */

/**
 * This enum stores delegate names. Module functions can be bound to these delegates by choosing options in this enum.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EActivationCue : uint8
{
	OnDamaged					UMETA(DisplayName = "OnDamaged", Tooltip = "When the part is damaged in any way"),
	OnRepaired					UMETA(DisplayName = "OnRepaired", Tooltip = "When the part is repaired in any way. This is different from fully repaired as you can repair a part one pixel at a time"),
	OnDestroyed					UMETA(DisplayName = "OnDestroyed", Tooltip = "When the part is completely destroyed"),
	OnFullyRepaired				UMETA(DisplayName = "OnFullyRepaired", Tooltip = "When the part becomes fully intact"),
	OnFunctionalityLost			UMETA(DisplayName = "OnFunctionalityLost", Tooltip = "When the part can no longer function"),
	OnFunctionalityRestored		UMETA(DisplayName = "OnFunctionalityRestored", Tooltip = "When the part gains enough pixels to function"),
	OnVoidsongCue				UMETA(DisplayName = "OnVoidsong", Tooltip = "When a Voidsong cue is called"),
};
ENUM_CLASS_FLAGS(EActivationCue);

/* /\ ============== /\ *\
|  /\ EActivationCue /\  |
\* /\ ============== /\ */

/* \/ ============ \/ *\
|  \/ EVoidsongCue \/  |
\* \/ ============ \/ */

/**
 * This enum stores delegate names specific to Voidsongs. Module functions can be bound to these delegates by choosing options in this enum.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EVoidsongCue : uint8
{
	Beat		UMETA(DisplayName = "Beat"),
	Tick		UMETA(DisplayName="Tick"),
};
ENUM_CLASS_FLAGS(EVoidsongCue)

/* /\ ============ /\ *\
|  /\ EVoidsongCue /\  |
\* /\ ============ /\ */

/* \/ =========== \/ *\
|  \/ UPartModule \/  |
\* \/ =========== \/ */

/**
 * A UPartModule defines functionality of a part. For example, there could be a laser module, a cannon module, an explode module, a thrust module, etc. The module's functionality is defined
 * in the "OnActivate" BlueprintImplementable function. Any given part could have multiple modules: for example, a cannon projectile might have a "projectile" module and a "explode" module.
 * The "Activate" function is bound to whatever delegates are selected in the EActivationCue enum.
 */
UCLASS(EditInlineNew, Blueprintable, HideDropdown)
class VOIDSINGER_API UPartModule : public UObject
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Initializes the part module's private variables
	 * 
	 * @param OwningPart - The part that this module is being initialized from
	 */
	void InitializeVariables(UPart* OwningPart);

private:

	// The part that this module is on
	UPart* Part;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------- *\
	\* \/ Activation \/ */

public:

	/**
	 * Allows for blueprint logic when the part module is activated
	 */
	UFUNCTION(BlueprintImplementable, DisplayName = ("Activate"), Category = ("Activation"))
	void OnActivate(TArray<TSubclassOf<UBaseVerbVoidsong>> Verbs, float Effectiveness);

	// \/ Activate \/

	/**
	 * Calls the "OnActivate" function so the part module's functionality is executed
	 * 
	 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
	 */
	void Activate(float Effectiveness);

	/**
	 * Activate overload - Checks whether "OnActivate" should be called by seeing if this module statisfies the Voidsong conditions. If it does, it calls the "OnActivate" function so the part module's functionality is executed
	 */
	void Activate(const TArray<EFactions>& Factions, const TArray<TSubclassOf<UPart>>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectiveness);

	// /\ Activate /\

	/* /\ Activation /\ *\
	\* ---------------- */

	/* ---------------------- *\
	\* \/ Delegate Binding \/ */

	/* /\ Delegate Binding /\ *\
	\* ---------------------- */

};

/* /\ =========== /\ *\
|  /\ UPartModule /\  |
\* /\ =========== /\ */