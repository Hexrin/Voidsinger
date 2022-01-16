// Fill out your copyright notice in the Description page of Project Settings.


#include "PartModule.h"

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