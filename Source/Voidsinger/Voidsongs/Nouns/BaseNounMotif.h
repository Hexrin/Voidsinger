// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Voidsinger/Voidsongs/BaseMotif.h"
#include "BaseNounMotif.generated.h"

/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UPart;

/* /\ Foward Declarations /\ *\
\* ------------------------- */

/* \/ ===== \/ *\
|  \/ ENoun \/  |
\* \/ ===== \/ */

/**
 * This enum stores Noun types.
 */
UENUM(BlueprintType)
enum class ENoun : uint8
{
	Unbound		UMETA(DisplayName = "Unbound"),
	Laser		UMETA(DisplayName = "Laser"),
	Cannon		UMETA(DisplayName = "Cannon"),
	Thruster	UMETA(DisplayName = "Thruster"),
};

/* /\ ===== /\ *\
|  /\ ENoun /\  |
\* /\ ===== /\ */

/**
 * The BaseNounVoidsong stores a Noun type on it. It is the base class for all Noun Voidsongs. When this type of Voidsong is performed, any other Voidsongs played will only apply to
 * module's with the noun type stored on this Voidsong (or any other Noun Voidsongs that were also played).
 */
UCLASS(Blueprintable)
class VOIDSINGER_API UBaseNounMotif : public UBaseMotif
{
	GENERATED_BODY()

public:

	/* ---------------- *\
	\* \/ Motif Data \/ */

	//The class of the noun that this Voidsong will activate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "MotifData"))
	ENoun Noun;

	/* /\ Motif Data /\ *\
	\* ---------------- */
};
