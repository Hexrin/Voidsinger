// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Voidsinger/Voidsongs/BaseMotif.h"
#include "Voidsinger/Voidsongs/Factions/BaseFactionMotif.h"
#include "Voidsinger/Voidsongs/Nouns/BaseNounMotif.h"
#include "Voidsinger/Voidsongs/Verbs/BaseVerbMotif.h"
#include "Voidsinger/VoidsingerTypes.h"
#include "PartModule.generated.h"


/**
 * A UPartModule defines functionality of a part.
 */
UCLASS(EditInlineNew, Blueprintable, HideDropdown)
class VOIDSINGER_API UPartModule : public UObject
{
	GENERATED_BODY()

	/* -------------------- *\
	\* \/ Initialization \/ */

public:

	/**
	 * Initializes the part module's protected and private variables
	 * 
	 * @param OwningPart - The part that this module is being initialized from
	 */
	void InitializeVariables(UPart* OwningPart);

protected:

	// The part that this module is on
	UPart* Part;

	/* /\ Initialization /\ *\
	\* -------------------- */

};

/* /\ =========== /\ *\
|  /\ UPartModule /\  |
\* /\ =========== /\ */