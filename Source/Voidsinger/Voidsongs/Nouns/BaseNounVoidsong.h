// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseVoidsong.h"
#include "BaseNounVoidsong.generated.h"

/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UBasePart;

/* /\ Foward Declarations /\ *\
\* ------------------------- */

/**
 * The BaseNounVoidsong stores a part class on it. It is the base class for all Noun Voidsongs. When this type of Voidsong is performed, any other Voidsongs played will only apply to
 * the part class stored on this Voidsong (or any other Noun Voidsongs that were also played).
 */
UCLASS(Abstract, Blueprintable)
class VOIDSINGER_API UBaseNounVoidsong : public UBaseVoidsong
{
	GENERATED_BODY()

public:

	/* ------------------- *\
	\* \/ Voidsong Data \/ */

	//The class of the noun that this Voidsong will activate
	UPROPERTY(EditDefaultsOnly, config, BlueprintReadOnly, meta = (Category = "Config|VoidsongData"))
	TSubclassOf<UPart> Noun;

	/* /\ VoidsongData /\ *\
	\* ------------------ */
};
