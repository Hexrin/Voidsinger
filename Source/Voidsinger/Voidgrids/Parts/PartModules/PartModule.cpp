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
}

/* /\ Initialization /\ *\
\* -------------------- */

/* ------------------- *\
\* \/ Functionality \/ */

/**
 * Updates any internal logic based on this parts functionality.
 */
void UPartModule::FunctionalityRestored()
{
}

/**
 * Updates any internal logic based on this parts functionality.
 */
void UPartModule::FunctionalityLost()
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