// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "Kismet/GameplayStatics.h"
#include "BaseFactionVoidsong.generated.h"

/**
 * The BaseFactionVoidsong stores a faction on it. It is the base class for all Faction Voidsongs. When this type of Voidsong is performed, any other Voidsongs played will only apply to
 * ship that are part of this Voidsong's faction (or any other Faction Voidsongs that were also played).
 */
UCLASS(Blueprintable)
class VOIDSINGER_API UBaseFactionVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()
	
	/* ------------------- *\
	\* \/ Voidsong Data \/ */

public:

	//The faction that this Voidsong will make all other Voidsongs apply to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "VoidsongData"))
	EFaction Faction;

	/* /\ VoidsongData /\ *\
	\* ------------------ */

};
