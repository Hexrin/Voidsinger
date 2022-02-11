// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PartModule.h"
#include "Voidsinger/Voidsongs/Voidsong.h"
#include "ActivatablePartModule.generated.h"


/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UPart;

/* /\ Forward Declarations /\ *\
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

/* \/ =================== \/ *\
|  \/ FPartActivationData \/  |
\* \/ =================== \/ */

USTRUCT(BlueprintType)
struct FPartActivationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UBaseVerbMotif*> Verbs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Effectiveness { 1 };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Vector = FVector2D::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Rotation { 0 };

	FPartActivationData(float ActivationEffectiveness = 1, FVector2D ActivationVector = FVector2D::ZeroVector, float ActivationRotation = 0, TArray<UBaseVerbMotif*> ActivationVerbs = TArray<UBaseVerbMotif*>())
	{
		Effectiveness = ActivationEffectiveness;
		Vector = ActivationVector;
		Rotation = ActivationRotation;
		Verbs = ActivationVerbs;
	}

	FPartActivationData(TArray<UBaseVerbMotif*> ActivationVerbs, float ActivationEffectiveness = 1, FVector2D ActivationVector = FVector2D::ZeroVector, float ActivationRotation = 0)
	{
		FPartActivationData(ActivationEffectiveness, ActivationVector, ActivationRotation, ActivationVerbs);
	}
};

/* /\ =================== /\ *\
|  /\ FPartActivationData /\  |
\* /\ =================== /\ */

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
	virtual void InitializeVariables(UPart* OwningPart) override;

	/* /\ Initialization /\ *\
	\* -------------------- */

	/* ---------------- *\
	\* \/ Activation \/ */

protected:

	/**
	 * Called when the owning part is activated and calls the functionality of this part.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, DisplayName = "Activate", Category = "Activation")
	void OnActivate(const FPartActivationData Data);
	virtual void OnActivate_Implementation(const FPartActivationData Data);

public:
	// \/ Activate \/ /

	/**
	 * Calls the "OnActivate" function with an Effectiveness of 1 so the part module's functionality is executed.
	 */
	void Activate(bool bApplyEffect = true);

	/**
	 * Calls the "OnActivate" function so the part module's functionality is executed
	 *
	 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Activate", Category = "Activation")
	void ActivateWithEffectiveness(float Effectiveness);

	/**
	 * Calls the "OnActivate" function so the part module's functionality is executed
	 *
	 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
	 * @param Vector - The direction to activate this part in.
	 * @param Rotation - The rotation of the effect of this voidsong.
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Activate", Category = "Activation")
	void ActivateWithEffectivenessVectorAndRotation(float Effectiveness, FVector2D Vector, float Rotation);

	/**
	 * Calls the "OnActivate" function with the Verbs played in a Voidsong so the part module's functionality of executed
	 * 
	 * @param Verbs - The Verbs played in the Voidsong
	 * @param Effectiveness - The effectiveness of the activation. Useful for when activate is called every tick
	 */
	void ActivateFromVoidsong(const TArray<UBaseVerbMotif*> Verbs, float Effectiveness);

	// /\ Activate /\ /

	//Stores what Noun Motif will activate this part module. If unbound, this module will not be activatable by a Voidsong.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Activation")
	ENoun Noun;

	/* /\ Activation /\ *\
	\* ---------------- */

	/* ---------------------- *\
	\* \/ Delegate Binding \/ */

public:
	/**
	 * Binds the activate function to all selected delegates
	 */
	void BindToDelegates();

	/**
	 * Checks whether to bind to the Voidsong given by seeing if this module statisfies the Voidsong conditions. If it does, ActivateWithEffectiveness is bound to the relavent VoidsongCues.
	 *
	 * @param Voidsong - The Voidsong to bind to
	 */
	void BindToVoidsong(UVoidsong* Voidsong);

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