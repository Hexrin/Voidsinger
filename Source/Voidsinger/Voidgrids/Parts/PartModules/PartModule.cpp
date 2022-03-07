// Fill out your copyright notice in the Description page of Project Settings.

#include "PartModule.h"

#include "Voidsinger/Voidgrids/Parts/Part.h"

/* -------------------- *\
\* \/ Initialization \/ */

/**
 * Initializes the part module's private variables
 *
 * @param OwningPart - The part that this module is being initialized from
 */
void UPartModule::InitializeVariables(UPart* OwningPart)
{
	Part = OwningPart;
	Part->OnFunctionalityRestored.AddUniqueDynamic(this, &UPartModule::FunctionalityRestored);
	Part->OnFunctionalityLost.AddUniqueDynamic(this, &UPartModule::FunctionalityLost);
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ------------------- *\
\* \/ Functionality \/ */

/**
 * Updates any internal logic based on this parts functionality.
 * 
 * @param bApplyChangeEffect - Whether to apply the effects of this change
 */
void UPartModule::FunctionalityRestored(bool bApplyChangeEffect)
{
}

/**
 * Updates any internal logic based on this parts functionality.
 * 
 * @param bApplyChangeEffect - Whether to apply the effects of this change
 */
void UPartModule::FunctionalityLost(bool bApplyChangeEffect)
{
}

/* /\ Functionality /\ *\
\* ------------------- */

/**
 * Gets the voidgrid this is a part of.
 * 
 * @return The voidgrid this is a part of.
 */
AVoidgrid* UPartModule::GetVoidgrid()
{
	return Part->GetVoidgrid();
}