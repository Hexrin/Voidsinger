// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PartModule.generated.h"

/* -------------------------- *\
\* \/ Forward Declarations \/ */

class UPart;
class AVoidgrid;

/* /\ Forward Declarations /\ *\
\* ------------------------- */

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
	virtual void InitializeVariables(UPart* OwningPart);

	/* /\ Initialization /\ *\
	\* -------------------- */


	/* ------------------- *\
	\* \/ Functionality \/ */

public:

	/**
	 * Updates any internal logic based on this parts functionality.
	 * 
	 * @param bApplyChangeEffect - Whether to apply the effects of this change
	 */
	UFUNCTION()
	virtual void FunctionalityRestored(bool bApplyChangeEffect);

	/**
	 * Updates any internal logic based on this parts functionality.
	 * 
	 * @param bApplyChangeEffect - Whether to apply the effects of this change
	 */
	UFUNCTION()
	virtual void FunctionalityLost(bool bApplyChangeEffect);

	/* /\ Functionality /\ *\
	\* ------------------- */

public:

	/* ---------------- *\
	\* \/ References \/ */

	/**
	 * Gets the voidgrid this is a part of.
	 *
	 * @return The voidgrid this is a part of.
	 */
	UFUNCTION(BlueprintPure)
	AVoidgrid* GetVoidgrid();

	// The part that this module is on
	UPart* Part;

	/* /\ References /\ *\
	\* ---------------- */
};

/* /\ =========== /\ *\
|  /\ UPartModule /\  |
\* /\ =========== /\ */