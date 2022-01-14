// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/Voidsongs/Factions/BaseFactionVoidsong.h"
#include "Voidsinger/Voidsongs/Nouns/BaseNounVoidsong.h"
#include "Voidsinger/Voidsongs/Verbs/BaseVerbVoidsong.h"
#include "PartModule.generated.h"

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

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable, HideDropdown)
class VOIDSINGER_API UPartModule : public UObject
{
	GENERATED_BODY()

	void Activate(float Effectivness);
	void Activate(const TArray<EFactions>& Factions, const TArray<TSubclassOf<UPart>>& Nouns, const TArray<TSubclassOf<UBaseVerbVoidsong>>& Verbs, const TArray<TSubclassOf<UBaseVoidsong>>& PlayableVoidsongs, float Effectivness);

	UFUNCTION(BlueprintImplementable, DisplayName = ("Activate"))
	void OnActivate(TArray<TSubclassOf<UBaseVerbVoidsong>> Verbs);


	//UPROPERTY(EditAnywhere)

};
