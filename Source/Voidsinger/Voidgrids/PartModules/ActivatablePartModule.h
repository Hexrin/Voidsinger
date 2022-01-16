// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartModule.h"
#include "ActivatablePartModule.generated.h"


/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UPart;

/* /\ Foward Declarations /\ *\
\* ------------------------- */

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
	OnFunctionalityLost			UMETA(DisplayName = "OnFunctionalityLost", Tooltip = "When the part can no longer function"),
	OnFunctionalityRestored		UMETA(DisplayName = "OnFunctionalityRestored", Tooltip = "When the part gains enough pixels to function"),
	OnDestroyed					UMETA(DisplayName = "OnDestroyed", Tooltip = "When the part is completely destroyed"),
	OnFullyRepaired				UMETA(DisplayName = "OnFullyRepaired", Tooltip = "When the part becomes fully intact"),
	OnTick						UMETA(DisplayName = "OnTick", Tooltip = "Every tick"),
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
	OnBeat			UMETA(DisplayName = "OnBeat"),
	ForDuration		UMETA(DisplayName = "ForDuration"),
};
ENUM_CLASS_FLAGS(EVoidsongCue)

/* /\ ============ /\ *\
|  /\ EVoidsongCue /\  |
\* /\ ============ /\ */

/* \/ ====================== \/ *\
|  \/ UActivatablePartModule \/  |
\* \/ ====================== \/ */

/**
 * This subclass of UPartModule is activatable. Functionality is defined in the "OnActivate" BlueprintImplementable function. 
 * The "Activate" function is bound to whatever delegates are selected in the EActivationCue and EVoidsongCue enums.
 */
UCLASS()
class VOIDSINGER_API UActivatablePartModule : public UPartModule
{
	GENERATED_BODY()
	

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Initializes the part module's protected and private variables. Overriden from PartModule to call BindToDelegates().
	 *
	 * @param OwningPart - The part that this module is being initialized from
	 */
	void InitializeVariables(UPart* OwningPart);

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------- *\
	\* \/ Activation \/ */

public:

	/**
	 * Allows for blueprint logic when the part module is activated
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Activate", Category = "Activation")
		void OnActivate(const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, float Effectiveness);

	// \/ Activate \/

	/**
	 * Calls the "OnActivate" function with an Effectiveness of 1 so the part module's functionality is executed.
	 */
	void Activate();

	/**
	 * Calls the "OnActivate" function so the part module's functionality is executed
	 *
	 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Activate", Category = "Activation")
	void ActivateWithEffectiveness(float Effectiveness);

	/**
	 * Checks whether "OnActivate" should be called by seeing if this module statisfies the Voidsong conditions. If it does, it calls the "OnActivate" function so the part module's functionality is executed
	 *
	 * @param Factions - The Factions that were activated
	 * @param Nouns - The Nouns that were activated
	 * @param Verbs - The Verbs that were activated
	 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
	 */
	void ActivateFromVoidsong(const TArray<EFaction>& Factions, const TArray<ENoun>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectiveness = 1);

	// /\ Activate /\

	//Stores what Noun Voidsong will activate this part module. If unbound, this module will not be activatable by a Voidsong.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	ENoun Noun;

	/* /\ Activation /\ *\
	\* ---------------- */

	/* ---------------------- *\
	\* \/ Delegate Binding \/ */

	/**
	 * Binds the activate function to all selected delegates
	 */
	void BindToDelegates();

	// The events to bind Activate to
	UPROPERTY(EditAnywhere, Category = "DelegateBinding")
	EActivationCue ActivationCues;

	// The Voidsong events to bind Activate to
	UPROPERTY(EditAnywhere, Category = "DelegateBinding")
	EVoidsongCue VoidsongCues;

	/* /\ Delegate Binding /\ *\
	\* ---------------------- */

};

/* /\ =========== /\ *\
|  /\ UPartModule /\  |
\* /\ =========== /\ */